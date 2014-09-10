#include <numeric>
#include <algorithm>
#include <calculator.h>

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

double calc_average_significant_period(vector<double> periods, size_t k)
{
    sort(periods.begin(),periods.end());
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

vector<double> gen_sin_vector(size_t length, double ampl, double period)
{
    auto f = [ampl, period](int t){return ampl*sin((double)t*2*M_PI/period)+ampl;};
    vector<double> res(length);
    for(size_t i = 0; i < length; i++)
        res[i] = f(static_cast<int>(i));
    return res;
}


vector<unique_ptr<Mat>> gen_test_image(size_t length, size_t width, size_t height, double ampl, double period)
{
    vector<unique_ptr<Mat>> res(length);

    for(size_t i = 0; i < length; i++)
        res[i] = unique_ptr<Mat>(new Mat(height,width));

    for(size_t i = 0; i < height; i++)
        for(size_t j = 0; j < width; j++)
        {
            vector<double> values {gen_sin_vector(length,ampl,period)};
            for(size_t k = 0; k < length; k++)
            {
                auto vec = res[k]->getVec(i,j);
                vec[0] = values[k];
                vec[1] = values[k];
                vec[2] = values[k];
            }
        }

    return res;
}


vector<double> receive_pixel_values(vector<unique_ptr<Mat>>& src, size_t row, size_t col, size_t channel, size_t start, size_t end)
{
    vector<double> res(src.size());

    if (end > res.size())
        throw out_of_range("End parameter is more than video length");

    for(auto k = start; k < end; k++)
        res[k]=src[k]->getVec(row,col)[channel];

    return res;
}


vector<double> receive_pixel_values(vector<unique_ptr<Mat>>& src, size_t row, size_t col, size_t channel)
{
    return receive_pixel_values(src,row,col,channel,0,src.size());
}


inline bool is_in_circle(size_t x, size_t y, double area_size)
{
    return ((double)(x*x+y*y) <= area_size*area_size);
}


vector<double> receive_averaged_pixel_values(vector<unique_ptr<Mat>>& src, size_t row, size_t col, double area_size)
{
    int count = 0;
    vector<double> res(src.size());
    auto istart = (row > static_cast<size_t>(area_size)) ? (row - static_cast<size_t>(area_size)) : 0;
    auto iend = row + static_cast<size_t>(area_size);
    auto jstart = (col > static_cast<size_t>(area_size)) ? (col - static_cast<size_t>(area_size)) : 0;
    auto jend = col + static_cast<size_t>(area_size);
    for(auto i = istart; i < iend; i++)
        for(auto j = jstart; j < jend; j++)
        {
           if (is_in_circle(j - col, i - row, area_size))
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



double calculate_pulse(VideoReader& video, vector<Point>& points, double fr1, double fr2, double ampFactor, size_t avg_parameter, double area_radius)
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


vector<Point> standart_points(size_t width, size_t height)
{
    vector<Point> res;
    for(int i = 1; i <= 3; i++)
        for(int j = 1; j <= 3; j++)
        {
            res.push_back(Point(width * i / 4, height * j / 4));
        }
    return res;
}


vector<Point> find_points_of_interest(VideoReader& video)
{
    const size_t min_points {3};
    vector<Point> res;
    //TODO
    if (res.size() < min_points)
        res = standart_points(video.getFrameWidth(),video.getFrameHeight());
    return res;
}

}
