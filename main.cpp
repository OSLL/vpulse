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
    char* filename_out = "face2";

    VideoReader* Curr_video=new VideoReader();
    Curr_video->CVReadVideo(filename_in);
    //Curr_video->ReadFrames(filename_in,4);
    //Curr_video->PrintFrames();
    int sRate=30; //TODO

    processor* Pr1= new processor(Curr_video->getNumberOfFrames(),Curr_video->getFrameHeight(),Curr_video->getFrameWidth(), sRate, Curr_video->getBluredFrames());

    float fr1=50.0/60.0;
    float fr2= 78.0/60.0;
    double ampFactor = 30.0;
    Pr1->work(fr1,fr2,ampFactor);

    double* summSignal;
    Pr1->AddPulseToFrames(Curr_video->getFrames(),summSignal,Curr_video->getNumberOfFrames());
    //Curr_video->PrintFrames();
    //Curr_video->CVWriteVideo(filename_out);
    //Curr_video->CVOutputVideo();
    printf("success!\n");
    printf("Height = %d\n",Curr_video->getFrameHeight());
    printf("Width = %d\n",Curr_video->getFrameWidth());
    delete(Pr1);
    delete(Curr_video);
    QPushButton B1("Stop",&w);
    QObject::connect(&B1,SIGNAL(clicked()),&a,SLOT(quit()));
    B1.show();
    QPushButton B2("Show",&w);
    B2.setGeometry(5,35,90,30);
    //QObject::connect(&B2,SIGNAL(clicked()),*Curr_video,SLOT(CVOutputVideo()));
    B1.show();
    //printf("end\n");
    return a.exec();
}

