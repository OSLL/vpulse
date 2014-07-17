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

using complex_number = complex<double>;
using comlex_array = vector<complex<double>>;
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


/*int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    w.show();
    double* srcIm;
    double* dstIm;
    int ow=2;
    int oh=2;
    int nw=50;
    int nh=50;
    int nofFr=2;
    int frInd=0;
    //initializeInterpolation(srcIm,dstIm,ow,nw,oh,nh,nofFr);
    srcIm=(double*)malloc(sizeof(double)*ow*oh*3*nofFr);
    dstIm=(double*)malloc(sizeof(double)*nw*nh*3*nofFr);
    for(int i=0; i<ow*oh*3*nofFr;i++)
    {
        srcIm[i] = (double)i;
        i++;
        srcIm[i] = 10.0;
    }
    for(int i=0; i<ow*oh*3*nofFr;i++)
    {
        dstIm[i] = -1.0;
    }
    char* filename_in = "face.mp4";
    VideoReader* Curr_video=new VideoReader();
    Curr_video->ReadFrames(filename_in,4);
    int sRate=30;

    processor* Pr1= new processor(Curr_video->getNumberOfFrames(),Curr_video->getFrameHeight(),Curr_video->getFrameWidth(), sRate, Curr_video->getBluredFrames());
    //printarray(srcIm,ow,oh,nofFr);
    Pr1->NearInterpolation(srcIm,dstIm,ow,oh,nw,nh,nofFr,frInd);
    frInd++;
    Pr1->NearInterpolation(srcIm,dstIm,ow,oh,nw,nh,nofFr,frInd);
    printarray(dstIm,nw,nh,nofFr);

    QPushButton B1("Stop",&w);
    QObject::connect(&B1,SIGNAL(clicked()),&a,SLOT(quit()));
    B1.show();
    //printf("end\n");
    return a.exec();
}
*/



harmonic_stat calc_amplitude_and_period(vector<double>& values)
{
    //cout << values.size() << endl;
    comlex_array out_fft(values.size());

    fftw_plan plan = fftw_plan_dft_r2c_1d((int)values.size(), values.data(), reinterpret_cast<fftw_complex*>(out_fft.data()),FFTW_MEASURE);
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
    assert(k<values.size()); //TODO: add checking

    std::sort(values.begin(),values.end(),comp);
    k--;
    int min_diff_pos = 0;
    for(int i = 0; i < (int)(values.size()- k); i++)
    {
        if (values[i + k].second - values[i].second < values[min_diff_pos + k].second - values[min_diff_pos].second)
            min_diff_pos = i;
    }

    double acc = 0;
    for(auto it = values.cbegin(); it != (values.cbegin() + k); it++)
        acc += it->second;
    return acc/k;
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    const string filename_in {"face.mp4"};

    VideoReader* Curr_video=new VideoReader();
    if (Curr_video->ReadFrames(filename_in.c_str(),4)<0)
    {
        cout << "Failed to read file" << endl;
        return -1;
    }
    //Curr_video->PrintFrames();
    int sRate=30; //TODO
    const string new_dir_name {"PulseSignal"};
    const string new_dir_name_fr_txt {"FramesInputTxt"};
    const string new_dir_name_frb_txt {"FramesBlurredTxt"};
    QDir new_dir(new_dir_name.c_str());
    QDir new_dir_fr_txt(new_dir_name_fr_txt.c_str());
    QDir new_dir_frb_txt(new_dir_name_frb_txt.c_str());
    if(!(new_dir.exists()))
    {
        new_dir.mkpath(".");
    }
    if(!(new_dir_fr_txt.exists()))
    {
        new_dir_fr_txt.mkpath(".");
    }
    if(!(new_dir_frb_txt.exists()))
    {
        new_dir_frb_txt.mkpath(".");
    }
    string f_name {"tmp1.log"};
    string f_dir_name;
    f_dir_name.resize(new_dir_name.length()+f_name.length()+1);

    processor* Pr1 = new processor(Curr_video->getNumberOfFrames(),Curr_video->getFrameHeight(),Curr_video->getFrameWidth(), sRate, Curr_video->getBluredFrames());

    const double fr1 {50.0/60.0};
    const double fr2 {78.0/60.0};
    const double ampFactor {70.0};

    Pr1->work(fr1,fr2,ampFactor);
//FIX //Pr1->VectorToFrames(Pr1->getAllFrames(),Curr_video->getBluredFrames(), Pr1->getFrW(),Pr1->getFrH(),Pr1->getNFr());
//FIX   //Curr_video->PyrUpBlured(4);
    double* summSignal;
    Pr1->AddPulseToFrames(Curr_video->getFrames()/*,Curr_video->getBluredFrames()*/,summSignal,Curr_video->getNumberOfFrames());
    //Pr1->VectorToFrames(summSignal,Curr_video->getBluredFrames(), Curr_video->getFrames()[0]->cols,Curr_video->getFrames()[0]->rows,Curr_video->getNumberOfFrames());
    //free(summSignal);
    //Curr_video->AddPulseToFrames();
    Curr_video->PrintFrames();

    //vector<double> values(frames,frames+framecount*sizeof(double));


    vector<double> values_array[3][3];
    vector<harmonic_stat> harmonic_stats;



    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
        {
            values_array[i][j]=Pr1->receive_pixel_values(i*Pr1->getFrH()/4,j*Pr1->getFrW()/4);
            //cout << values_array[i][j].size() << endl;
            harmonic_stats.push_back(calc_amplitude_and_period(values_array[i][j]));
        }

    const int avg_parameter = 3;
    double pulse = calc_average_significant_pulse(harmonic_stats,avg_parameter);
    cout << pulse << endl;

    delete(Pr1);
    delete(Curr_video);
    delete(summSignal);

    return 0;
}

