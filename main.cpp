#include "Conf.h"
#include "videoreader.h"
#include "processor.h"

using namespace cv;
using namespace std;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    char* filename_in = "face.mp4";


    VideoReader* Curr_video=new VideoReader();
    //Curr_video->ReadFrames(filename_in,4);
    //int sRate=30; //TODO
    Curr_video->CVReadVideo(filename_in);
/*
    char*f_name="tmp1.log";
    char f_dir_name[strlen(new_dir_name)+strlen(f_name)+1];
    processor* Pr1= new processor(Curr_video->getNumberOfFrames(),Curr_video->getFrameHeight(),Curr_video->getFrameWidth(), sRate, Curr_video->getBluredFrames());
    float fr1=50.0/60.0;
    float fr2= 78.0/60.0;
    double ampFactor = 70.0;
    Pr1->work(fr1,fr2,ampFactor);
    double* summSignal;
    Pr1->AddPulseToFrames(Curr_video->getFrames(),summSignal,Curr_video->getNumberOfFrames());
    Curr_video->PrintFrames();
    printf("success!\n");
    printf("Height = %d\n",Curr_video->getFrameHeight());
    printf("Width = %d\n",Curr_video->getFrameWidth());
    delete(Pr1);
    */
    delete(Curr_video);
    QPushButton B1("Stop",&w);
    QObject::connect(&B1,SIGNAL(clicked()),&a,SLOT(quit()));
    B1.show();
    //printf("end\n");
    return a.exec();
}

