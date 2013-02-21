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
    Curr_video->ReadFrames(filename_in,4);
    //Curr_video->PrintFrames();
    int sRate=30; //TODO

    processor* Pr1= new processor(Curr_video->getNumberOfFrames(),Curr_video->getFrameHeight(),Curr_video->getFrameWidth(), sRate, Curr_video->getFrames());
    const char* f_name_AllFramesR = "AllFramesR.log";
    const char* f_name_AllFramesG = "AllFramesG.log";
    const char* f_name_AllFramesB = "AllFramesB.log";
    Pr1->PrintData(&Pr1->getAllFrames()[0], Pr1->getFrH()*Pr1->getFrW()*Pr1->getNFr(), f_name_AllFramesR);
    Pr1->PrintData(&Pr1->getAllFrames()[Pr1->getFrH()*Pr1->getFrW()*Pr1->getNFr()], Pr1->getFrH()*Pr1->getFrW()*Pr1->getNFr(), f_name_AllFramesG);
    Pr1->PrintData(&Pr1->getAllFrames()[Pr1->getFrH()*Pr1->getFrW()*Pr1->getNFr()*2], Pr1->getFrH()*Pr1->getFrW()*Pr1->getNFr(), f_name_AllFramesB);
    const char* f_name_AllFramesY = "AllFramesY.log";
    const char* f_name_AllFramesI = "AllFramesI.log";
    const char* f_name_AllFramesQ = "AllFramesQ.log";
    float fr1=50.0/60.0;
    float fr2= 60.0/60.0;
    //printf("flow= %f, fHight= %f \n", fLow, fHight);
    double ampFactor = 30.0;
    Pr1->work(fr1,fr2,ampFactor);
    Pr1->VectorToFrames(Pr1->getAllFrames(),Curr_video->getFrames());
    Curr_video->ChangePyramidLevel(4);
    Curr_video->PrintFrames();
    //Pr1->PrintData(&Pr1->getAllFrames()[0], Pr1->getFrH()*Pr1->getFrW()*Pr1->getNFr(), f_name_AllFramesY);
    //Pr1->PrintData(&Pr1->getAllFrames()[Pr1->getFrH()*Pr1->getFrW()*Pr1->getNFr()], Pr1->getFrH()*Pr1->getFrW()*Pr1->getNFr(), f_name_AllFramesI);
    //Pr1->PrintData(&Pr1->getAllFrames()[Pr1->getFrH()*Pr1->getFrW()*Pr1->getNFr()*2], Pr1->getFrH()*Pr1->getFrW()*Pr1->getNFr(), f_name_AllFramesQ);
    printf("success!\n");
    printf("Height = %d\n",Curr_video->getFrameHeight());
    printf("Width = %d\n",Curr_video->getFrameWidth());
    QPushButton B1("Stop",&w);
    QObject::connect(&B1,SIGNAL(clicked()),&a,SLOT(quit()));
    B1.show();
    //printf("end\n");
    return a.exec();
}

