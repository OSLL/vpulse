#include<numeric>
#include<algorithm>
#include "Calculator.h"

using namespace std;

namespace Calculator
{

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

double calc_average_significant_period(vector<double> periods, unsigned int k)
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
    double acc = std::accumulate(periods.begin()+min_diff_pos,periods.begin()+min_diff_pos+k,0.0);
    return acc/k;
}

vector<double> gen_sin_vector(unsigned int length, double ampl, double period)
{
    auto f = [ampl, period](int t){return ampl*sin((double)t*2*M_PI/period)+ampl;};
    vector<double> res(length);
    for(unsigned int i = 0; i < length; i++)
        res[i] = f(i);
    return res;
}


vector<unique_ptr<Mat>> gen_test_image(unsigned int length, unsigned int width, unsigned int height, double ampl, double period)
{
    vector<unique_ptr<Mat>> res(length);

    for(unsigned int i = 0; i < length; i++)
        res[i] = unique_ptr<Mat>(new Mat(height,width));

    for(unsigned int i = 0; i < height; i++)
        for(unsigned int j = 0; j < width; j++)
        {
            vector<double> values {gen_sin_vector(length,ampl,period)};
            for(unsigned int k = 0; k < length; k++)
            {
                auto vec = res[k]->getVec(i,j);
                vec[0] = values[k];
                vec[1] = values[k];
                vec[2] = values[k];
            }
        }

    return res;
}


vector<double> receive_pixel_values(vector<unique_ptr<Mat>>& src, unsigned int row, unsigned int col, unsigned int channel)
{
    vector<double> res(src.size());

    for(int k = 0; k < (int)res.size(); k++)
        res[k]=src[k]->getVec(row,col)[channel];

    return res;
}


bool is_in_circle(double x, double y, double area_size)
{
    return ((x*x+y*y) <= area_size*area_size);
}


vector<double> receive_averaged_pixel_values(vector<unique_ptr<Mat>>& src, unsigned int row, unsigned int col, double area_size)
{
    int count = 0;
    vector<double> res(src.size());
    for(auto i = row - static_cast<unsigned int>(area_size); i < row + static_cast<unsigned int>(area_size); i++)
        for(auto j = col - static_cast<unsigned int>(area_size); j < col + static_cast<unsigned int>(area_size); j++)
        {
            if (i >= 0 && j >= 0)
                if (is_in_circle(j-col,i-row,area_size))
                {
                    vector<double> values_array_r{receive_pixel_values(src,row,col,0)};
                    vector<double> values_array_g{receive_pixel_values(src,row,col,1)};
                    vector<double> values_array_b{receive_pixel_values(src,row,col,2)};
                    vector<double> monochrome_values(values_array_b.size());
                    for(int i = 0; i < values_array_b.size(); i++)
                        monochrome_values[i] = sqrt((values_array_b[i]*values_array_b[i]+values_array_g[i]*values_array_g[i]+values_array_r[i]*values_array_r[i])/3);
                    transform(res.begin(), res.end(), monochrome_values.begin(), res.begin(), plus<double>());
                    count++;
                }
        }
    for(auto& x : res)
        x /= count;

    return res;
}



double calculate_pulse(VideoReader& video, vector<Point>& points, double fr1, double fr2, double ampFactor, unsigned int avg_parameter, double area_radius)
{
    const int sRate {30};
    Processor pr1(sRate, video.getBluredFrames());

    pr1.amplify(fr1,fr2,ampFactor);
    pr1.AddPulseToFrames(video.getFrames());

    vector<harmonic_stat> harmonic_stats;

    for(auto p : points)
    {
        vector<double> values {receive_averaged_pixel_values(video.getFrames(),p.second,p.first,area_radius)};
        harmonic_stats.push_back(calc_amplitude_and_period(values));
    }

    vector<double> periods(harmonic_stats.size());
    transform(harmonic_stats.begin(),harmonic_stats.end(),periods.begin(),[](harmonic_stat a){return a.second;}); //extract periods

    for(auto x : periods)
        cout << x << " ";
    cout << endl;

    double pulse = calc_average_significant_period(periods,avg_parameter)/sRate*60;

    return pulse;
}


vector<Point> standart_points(unsigned int width, unsigned int height)
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
    const unsigned int min_points {3};
    vector<Point> res;
    //TODO
    if (res.size() < min_points)
        res = standart_points(video.getFrameWidth(),video.getFrameHeight());
    return res;
}

}
