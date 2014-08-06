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


vector<double> receive_averaged_pixel_values(vector<unique_ptr<Mat>>& src, int NumberOfFrames, int row, int col, double area_size)
{
    int count = 0;
    vector<double> res(NumberOfFrames);
    for(int i = row - area_size; i < row + area_size; i++)
        for(int j = col - area_size; j < col + area_size; j++)
        {
            if (((i-row)*(i-row)+(j-col)*(j-col)) <= area_size*area_size)
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


int main(int argc, char *argv[])
{
    const string filename_in {"face.mp4"};
    av_register_all();
    VideoReader* Curr_video=new VideoReader();
    if (Curr_video->ReadFrames(filename_in,4,1000)<0)
    {
        cout << "Failed to read file" << endl;
        return -1;
    }

    const int sRate {30};
    processor* Pr1 = new processor(Curr_video->getNumberOfFrames(), sRate, Curr_video->getBluredFrames());

    /*int length = 200;
    int width = 500;
    int height = 500;
    double ampl = 50;
    double period = 30;
    Mat** test_image = gen_test_image(length,width,height,ampl,period);*/

    const double fr1 {50.0/60.0};
    const double fr2 {78.0/60.0};
    const double ampFactor {70.0};

    Pr1->amplify(fr1,fr2,ampFactor);
    Pr1->AddPulseToFrames(Curr_video->getFrames(),Curr_video->getNumberOfFrames());

    Curr_video->PrintFrames();
//    vector<double> values_array_r,values_array_g,values_array_b;
    vector<harmonic_stat> harmonic_stats;

    for(int i = 1; i <= 3; i++)
        for(int j = 1; j <= 3; j++)
        {
            //values_array_r=receive_pixel_values(Curr_video->getFrames(),Curr_video->getNumberOfFrames(),i*Curr_video->getFrameHeight()/4,j*Curr_video->getFrameWidth()/4,0);
            //values_array_g=receive_pixel_values(Curr_video->getFrames(),Curr_video->getNumberOfFrames(),i*Curr_video->getFrameHeight()/4,j*Curr_video->getFrameWidth()/4,1);
            //values_array_b=receive_pixel_values(Curr_video->getFrames(),Curr_video->getNumberOfFrames(),i*Curr_video->getFrameHeight()/4,j*Curr_video->getFrameWidth()/4,2);
            vector<double> values {receive_averaged_pixel_values(Curr_video->getFrames(),Curr_video->getNumberOfFrames(),i*Curr_video->getFrameHeight()/4,j*Curr_video->getFrameWidth()/4,3.0)};

            //values_array_r=receive_pixel_values(test_image,length,i*height/4,j*width/4,0);
            //values_array_g=receive_pixel_values(test_image,length,i*height/4,j*width/4,1);
            //values_array_b=receive_pixel_values(test_image,length,i*height/4,j*width/4,2);
            /*for(auto x : values)
                cout << x << " ";
            cout << endl;*/

            harmonic_stats.push_back(calc_amplitude_and_period(values));
        }


    for(auto x : harmonic_stats)
        cout << x.second << " ";
    cout << endl;

    const int avg_parameter = 3;
    vector<double> periods(harmonic_stats.size());
    std::transform(harmonic_stats.begin(),harmonic_stats.end(),periods.begin(),[](harmonic_stat a){return a.second;});
    double pulse = calc_average_significant_period(periods,avg_parameter);

    cout << pulse/sRate*60 << endl;

    //delete Pr1;
    //delete Curr_video;

    /*auto test = gen_sin_vector(280,80,4.5);
    for(auto x : test)
        cout << x << " ";
    cout << endl;

    auto result = calc_amplitude_and_period(test);
    cout << result.first << " " << result.second << endl;*/

    return 0;
}

