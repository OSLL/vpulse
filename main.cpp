#include "Conf.h"
#include "videoreader.h"
#include "processor.h"

#include <QCoreApplication>

using namespace cv;
using namespace std;

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

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    const string filename_in {"face.mp4"};

    VideoReader* Curr_video=new VideoReader();
    if (Curr_video->ReadFrames(filename_in.c_str(),4)<0)
    {
        cout << "Failed to read file" << endl;
        return 0;
    }
    Curr_video->PrintFrames();
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

    //f_dir_name[0]='\0';
    //strcat(f_dir_name,new_dir_name);
    //char* slesh = new char('92');
    //strcat(f_dir_name,slesh);
    //f_dir_name[strlen(new_dir_name)]=(char)47;g
    //strcat(f_dir_name,f_name);
    //sprintf(f_dir_name,new_dir_name);
    //printf(f_dir_name);
    //printf("\n");
    processor* Pr1= new processor(Curr_video->getNumberOfFrames(),Curr_video->getFrameHeight(),Curr_video->getFrameWidth(), sRate, Curr_video->getBluredFrames());
    //const char* f_name_AllFramesR = "AllFramesR.log";
    //const char* f_name_AllFramesG = "AllFramesG.log";
    //const char* f_name_AllFramesB = "AllFramesB.log";
    //Pr1->PrintData(&Pr1->getAllFrames()[0], Pr1->getFrH()*Pr1->getFrW()*Pr1->getNFr(), "PulseSignal/tmp1.log");
    //Pr1->PrintData(&Pr1->getAllFrames()[Pr1->getFrH()*Pr1->getFrW()*Pr1->getNFr()], Pr1->getFrH()*Pr1->getFrW()*Pr1->getNFr(), f_name_AllFramesG);
    //Pr1->PrintData(&Pr1->getAllFrames()[Pr1->getFrH()*Pr1->getFrW()*Pr1->getNFr()*2], Pr1->getFrH()*Pr1->getFrW()*Pr1->getNFr(), f_name_AllFramesB);
    //const char* f_name_AllFramesY = "AllFramesY.log";
    //const char* f_name_AllFramesI = "AllFramesI.log";
    //const char* f_name_AllFramesQ = "AllFramesQ.log";
    double fr1 {50.0/60.0};
    double fr2 {78.0/60.0};
    //printf("flow= %f, fHight= %f \n", fLow, fHight);
    double ampFactor = 70.0;
    Pr1->work(fr1,fr2,ampFactor);
//FIX //Pr1->VectorToFrames(Pr1->getAllFrames(),Curr_video->getBluredFrames(), Pr1->getFrW(),Pr1->getFrH(),Pr1->getNFr());
//FIX   //Curr_video->PyrUpBlured(4);
    double* summSignal;
    Pr1->AddPulseToFrames(Curr_video->getFrames()/*,Curr_video->getBluredFrames()*/,summSignal,Curr_video->getNumberOfFrames());
    //Pr1->VectorToFrames(summSignal,Curr_video->getBluredFrames(), Curr_video->getFrames()[0]->cols,Curr_video->getFrames()[0]->rows,Curr_video->getNumberOfFrames());
    //free(summSignal);
    //Curr_video->AddPulseToFrames();
    Curr_video->PrintFrames();

    double* frames {Pr1->getAllFrames()};
    int framecount {Pr1->getNFr()};

    vector<double> values(frames,frames+framecount*sizeof(double));
    delete(frames);




    //Pr1->PrintData(&Pr1->getAllFrames()[0], Pr1->getFrH()*Pr1->getFrW()*Pr1->getNFr(), f_name_AllFramesY);
    //Pr1->PrintData(&Pr1->getAllFrames()[Pr1->getFrH()*Pr1->getFrW()*Pr1->getNFr()], Pr1->getFrH()*Pr1->getFrW()*Pr1->getNFr(), f_name_AllFramesI);
    //Pr1->PrintData(&Pr1->getAllFrames()[Pr1->getFrH()*Pr1->getFrW()*Pr1->getNFr()*2], Pr1->getFrH()*Pr1->getFrW()*Pr1->getNFr(), f_name_AllFramesQ);
    printf("success!\n");
    printf("Height = %d\n",Curr_video->getFrameHeight());
    printf("Width = %d\n",Curr_video->getFrameWidth());
    delete(Pr1);
    delete(summSignal);
    delete(Curr_video);
    //QPushButton B1("Stop",&w);
    //QObject::connect(&B1,SIGNAL(clicked()),&a,SLOT(quit()));
    //B1.show();
    //printf("end\n");
    //return a.exec();



    return 0;
}

