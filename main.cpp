#include "Conf.h"
#include "videoreader.h"
#include "processor.h"

#include <QCoreApplication>

using namespace cv;
using namespace std;

#include<utility>
#include<vector>
#include<numeric>
#include<algorithm>


using harmonic_stat = pair<double,double>;

bool comp(harmonic_stat a, harmonic_stat b)
{
    return (a.second<b.second);
}


void initializeInterpolation(double* src, double* dst, int ow, int nw, int oh, int nh, int nofFr)
{
    src=(double*)malloc(sizeof(double)*ow*oh*3*nofFr);
    printf("oldsize= %d",ow*oh*3*nofFr);
    dst=(double*)malloc(sizeof(double)*nw*nh*3*nofFr);
    printf("newsize= %d",nw*nh*3*nofFr);
    for(int i=0; i<ow*oh*3*nofFr;i++)
    {
        src[i] = (double)i;
    }
}

void printarray(double* src, int w, int h, int noffr)
{
    for(int i=0; i<w*h*3*noffr; i++)
        printf("[%d]%lf\n",i,src[i]);
}

harmonic_stat calc_amplitude_and_period(vector<double>& values)
{
    //cout << values.size() << endl;
    complex_vector out_fft(values.size());

    fftw_plan plan = fftw_plan_dft_r2c_1d((int)values.size(), values.data(), reinterpret_cast<fftw_complex*>(out_fft.data()), FFTW_MEASURE);
    fftw_execute(plan);

    complex_number max;
    for(const auto& c : out_fft)
    {
        if (norm(c)>norm(max))
            max = c;
    }

    return make_pair(norm(max)/out_fft.size(),arg(max));
}

double calc_average_significant_pulse(vector<harmonic_stat> values, int k)
{
    std::sort(values.begin(),values.end(),comp);
    k--;
    int min_diff_pos = 0;
    for(int i = 0; i < (int)(values.size()- k); i++)
    {
        if (values[i + k].second - values[i].second < values[min_diff_pos + k].second - values[min_diff_pos].second)
            min_diff_pos = i;
    }

    double acc{0};
    for(auto it = values.cbegin(); it != (values.cbegin() + k); it++)
        acc += it->second;
    return acc/k;
}


int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    const string filename_in {"face.mp4"};

    VideoReader* Curr_video=new VideoReader();
    if (Curr_video->ReadFrames(filename_in,4)<0)
    {
        cout << "Failed to read file" << endl;
        return -1;
    }

    int sRate=30; //TODO
    processor* Pr1 = new processor(Curr_video->getNumberOfFrames(),Curr_video->getFrameHeight(),Curr_video->getFrameWidth(), sRate, Curr_video->getBluredFrames());

    const double fr1 {50.0/60.0};
    const double fr2 {78.0/60.0};
    const double ampFactor {70.0};


    Pr1->work(fr1,fr2,ampFactor);
    //Pr1->PrintData(Pr1->getAllFrames(),100,"new.log");
    Pr1->AddPulseToFrames(Curr_video->getFrames(),Curr_video->getNumberOfFrames());
    //Curr_video->PrintFrames();

    /*vector<double> values = Pr1->receive_pixel_values(Pr1->getFrH()/2,Pr1->getFrW()/2,0);

    for(auto val : values)
        cout << val << " ";*/

    vector<double> values_array[3][3];
    vector<harmonic_stat> harmonic_stats;

    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
        {
            values_array[i][j]=Pr1->receive_pixel_values(i*Pr1->getFrH()/4,j*Pr1->getFrW()/4,0);
            harmonic_stats.push_back(calc_amplitude_and_period(values_array[i][j]));
        }

    cout << harmonic_stats.size() << endl;
    for(auto x : harmonic_stats)
        cout << x.second << " ";
    cout << endl;

    const int avg_parameter = 3;
    double pulse = calc_average_significant_pulse(harmonic_stats,avg_parameter);

    cout << pulse << endl;

    delete Pr1;
    delete Curr_video;

    return 0;
}

