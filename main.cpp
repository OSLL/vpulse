#include "Conf.h"
#include "videoreader.h"
#include "processor.h"
#include "Mat.cpp"

#include <QCoreApplication>

using namespace std;

#include<utility>
#include<vector>
#include<numeric>
#include<algorithm>


using harmonic_stat = pair<double,double>;
using Point = pair<int,int>;

harmonic_stat calc_amplitude_and_period(vector<double> values)
{
    complex_vector out_fft(values.size());

    fftw_plan plan = fftw_plan_dft_r2c_1d((int)values.size(), values.data(), reinterpret_cast<fftw_complex*>(out_fft.data()), FFTW_ESTIMATE);
    fftw_execute(plan);

    int max = 1;
    for(int i = 1; i < out_fft.size(); i++)
    {
        if (norm(out_fft[i])>norm(out_fft[max]))
            max = i;
    }

    return make_pair((double)abs(out_fft[max])/out_fft.size(),(double)out_fft.size()/max);
}

double calc_average_significant_period(vector<double> periods, int k)
{
    std::sort(periods.begin(),periods.end());
    k--;
    int min_diff_pos = 0;
    for(int i = 0; i < (int)(periods.size()- k); i++)
    {
        if (periods[i + k] - periods[i] < periods[min_diff_pos + k] - periods[min_diff_pos])
            min_diff_pos = i;
    }
    k++;
    double acc = std::accumulate(periods.begin()+min_diff_pos,periods.begin()+min_diff_pos+k,0);
    return acc/k;
}

vector<double> gen_sin_vector(int length, double ampl, double period)
{
    auto f = [ampl, period](int t){return ampl*sin((double)t*2*M_PI/period)+ampl;};
    vector<double> res(length);
    for(int i = 0; i < length; i++)
        res[i] = f(i);
    return res;
}


Mat** gen_test_image(int length, int width, int height, double ampl, double period)
{
    auto res = new Mat*[length];

    for(int i = 0; i < length; i++)
        res[i] = new Mat(height,width,3);

    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
        {
            vector<double> values {gen_sin_vector(length,ampl,period)};
            for(int k = 0; k < length; k++)
            {
                auto vec = res[k]->getVec(i,j);
                vec[0] = values[k];
                vec[1] = values[k];
                vec[2] = values[k];
            }
        }

    return res;
}


vector<double> receive_pixel_values(vector<unique_ptr<Mat>>& src, int NumberOfFrames, int row, int col, int channel)
{
    vector<double> res(NumberOfFrames);

    for(int k = 0; k < (int)res.size(); k++)
        res[k]=src[k]->getVec(row,col)[channel];

    return res;
}


bool is_in_circle(double x, double y, double area_size)
{
    return ((x*x+y*y) <= area_size*area_size);
}


vector<double> receive_averaged_pixel_values(vector<unique_ptr<Mat>>& src, int NumberOfFrames, int row, int col, double area_size)
{
    int count = 0;
    vector<double> res(NumberOfFrames);
    for(int i = row - area_size; i < row + area_size; i++)
        for(int j = col - area_size; j < col + area_size; j++)
        {
            if (i >= 0 && j >= 0)
                if (is_in_circle(j-col,i-row,area_size))
                {
                    vector<double> values_array_r{receive_pixel_values(src,NumberOfFrames,row,col,0)};
                    vector<double> values_array_g{receive_pixel_values(src,NumberOfFrames,row,col,1)};
                    vector<double> values_array_b{receive_pixel_values(src,NumberOfFrames,row,col,2)};
                    vector<double> monochrome_values(values_array_b.size());
                    for(int i = 0; i < values_array_b.size(); i++)
                        monochrome_values[i] = sqrt((values_array_b[i]*values_array_b[i]+values_array_g[i]*values_array_g[i]+values_array_r[i]*values_array_r[i])/3);
                    std::transform(res.begin(), res.end(), monochrome_values.begin(), res.begin(), std::plus<double>());
                    count++;
                }
        }
    for(auto& x : res)
        x /= count;

    return res;
}



double calculate_pulse(VideoReader& video, vector<Point>& points, double fr1, double fr2, double ampFactor, int avg_parameter, double area_radius)
{
    const int sRate {30};
    processor Pr1(video.getNumberOfFrames(), sRate, video.getBluredFrames());

    Pr1.amplify(fr1,fr2,ampFactor);
    Pr1.AddPulseToFrames(video.getFrames(), video.getNumberOfFrames());

    vector<harmonic_stat> harmonic_stats;

    for(auto p : points)
    {
        vector<double> values {receive_averaged_pixel_values(video.getFrames(),video.getNumberOfFrames(),p.second,p.first,area_radius)};
        harmonic_stats.push_back(calc_amplitude_and_period(values));
    }

    vector<double> periods(harmonic_stats.size());
    std::transform(harmonic_stats.begin(),harmonic_stats.end(),periods.begin(),[](harmonic_stat a){return a.second;}); //extract periods

    double pulse = calc_average_significant_period(periods,avg_parameter)/sRate*60;

    return pulse;
}


vector<Point> standart_points(int width, int height)
{
    vector<Point> res;
    for(int i = 1; i <= 3; i++)
        for(int j = 1; j <= 3; j++)
        {
            res.push_back(Point(width*i/4,height*j/4));
        }
    return res;
}


vector<Point> find_points_of_interest(VideoReader& video)
{
    const int min_points {3};
    vector<Point> res;
    //TODO
    if (res.size() < min_points)
        res = standart_points(video.getFrameWidth(),video.getFrameHeight());
    return res;
}

int main()
{
    const string filename_in {"face.mp4"};
    av_register_all();
    const int frames_max {1000};
    VideoReader Curr_video;
    if (Curr_video.ReadFrames(filename_in,4,frames_max)<0)
    {
        cout << "Failed to read file" << endl;
        return -1;
    }

    const double fr1 {50.0/60.0};
    const double fr2 {78.0/60.0};
    const double ampFactor {70.0};
    const int avg_parameter {3};
    const double area_radius {3.0};

    vector<Point> points{find_points_of_interest(Curr_video)};

    cout << calculate_pulse(Curr_video,points,fr1,fr2,ampFactor,avg_parameter,area_radius) << endl;
    return 0;
}

