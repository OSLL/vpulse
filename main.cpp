#include "Conf.h"
#include "videoreader.h"
#include "processor.h"
#include <QTime>

using namespace cv;
using namespace std;

void PrintDataDb_(double* src, long len, const char* filename)
{
        FILE * stream;
                if ((stream=fopen(filename, "w")) != 0)
        {

            for(long curr_id1 = 0; curr_id1 <= len-1; curr_id1++)
            {

                fprintf(stream, "%ld. \t %lf",curr_id1, src[curr_id1]);
                fputc('\n',stream);

            };
        }
        fclose(stream);
}



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    char* filename_in = "palm.MOV";
    //char* filename_out = "face2";

    VideoReader* Curr_video=new VideoReader();
    processor* Pr1=new processor();
    float fr1= 50.0/60.0;
    float fr2= 75.0/60.0;
    double ampFactor = 10.0;
    //Pr1->init(90,37,33,30);
    //Curr_video->CVReadVideoCAM();
    //Curr_video->CVReadVideoCAM(filename_in);
    Curr_video->CVReadVideoRT(filename_in,Pr1,fr1,fr2,ampFactor);
    //Curr_video->ReadFrames(filename_in,4);
    //Curr_video->PrintFrames();
    //int sRate=30; //TODO
    //qDebug("SRate=%lf",Curr_video->getfps());
    //processor* Pr1= new processor(Curr_video->getNumberOfFrames(),Curr_video->getFrameHeight(),Curr_video->getFrameWidth(), sRate, Curr_video->getBluredFrames());
    //processor* Pr1= new processor(Curr_video->get_portion(),Curr_video->getFrameHeight(),Curr_video->getFrameWidth(), Curr_video->getfps(), Curr_video->getBluredFrames());


    //processor* Pr1= new processor();
    //Pr1->init(Curr_video->get_portion(),Curr_video->getFrameHeight(),Curr_video->getFrameWidth(), Curr_video->getfps()/*,Curr_video->getBluredFrames()*/);
    //QTime tt;
    //tt.start();
    //Pr1->FramesToVector(Curr_video->getBluredFrames(),Pr1->getAllFrames(),Pr1->getFrW(),Pr1->getFrH(),Pr1->getNFr());
    //qDebug("time elapsed: %d ms",tt.elapsed());
    //processor* Pr1=new processor();
    //Curr_video->createProcessor(Pr1);


    /*for(int g=0; g<Pr1->getNFr();g++)
    {
        Pr1->FrameToVector(Curr_video->getBluredFrames(),Pr1->getAllFrames(),Pr1->getFrW(),Pr1->getFrH(),Pr1->getNFr(),g);
    }*/
    //const char* fnfn = "tmp2.txt";
    //PrintDataDb_(Pr1->getAllFrames(),Pr1->getNFr()*Pr1->getFrH()*Pr1->getFrW()*3,fnfn);


    //Pr1->work(fr1,fr2,ampFactor);


    //double rate;
    ///Pr1->countPulseRate(&rate);
    //qDebug("Rate=%lf",rate);
    //double* summSignal;
    //Pr1->AddPulseToFrames(Curr_video->getFrames(),summSignal,Curr_video->getNumberOfFrames());
    //Curr_video->tmpframereader(filename_in);
    //int i=0;
    /*while(Curr_video->getNumberOfFrames()>Curr_video->get_portion()*(i+1))
    {
        QTime tt;
        tt.start();
        Pr1->AddPulseToFrames(&Curr_video->getFrames()[Curr_video->get_portion()*i],summSignal,Curr_video->get_portion());
        qDebug("time elapsed: %d ms",tt.elapsed());
        i++;
    }*/
    Pr1->freeRendBuff();

    //Pr1->AddPulseToFrames(Curr_video->getFrames(),summSignal,Curr_video->get_portion());
    //Pr1->AddPulseToFrames(&Curr_video->getFrames()[Curr_video->get_portion()+1],summSignal,Curr_video->get_portion());
    //Curr_video->PrintFrames();
    //Curr_video->CVWriteVideo(filename_out);
    //Curr_video->CVOutputVideo();
    //printf("success!\n");
    //printf("Height = %d\n",Curr_video->getFrameHeight());
    //printf("Width = %d\n",Curr_video->getFrameWidth());
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

