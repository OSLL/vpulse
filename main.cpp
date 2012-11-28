#include <QtGui/QApplication>
#include<QtGui/QPainter>
#include<QtGui/QLabel>
#include<QtGui/QPushButton>
#include<QtGui/QPixmap>
#include "mainwindow.h"
#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif

#include <stdio.h>
#include <cstdio>
using namespace std;
extern "C" {
#include <avcodec.h>
#include <avformat.h>
//#include"libavutil/samplefmt.h"
//#include"libavutil/common.h"
#include <swscale.h>
#include <version.h>        //swscale
}

#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame) {
  FILE *pFile;
  char szFilename[32];
  int  y;

  // Open file
  sprintf(szFilename, "frame%d.ppm", iFrame);
  pFile=fopen(szFilename, "wb");
  if(pFile==NULL)
    return;

  // Write header
  fprintf(pFile, "P6\n%d %d\n255\n", width, height);

  // Write pixel data
  for(y=0; y<height; y++)
    fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);

  // Close file
  fclose(pFile);
}



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.resize(100,80);
    /*QWidget* wgt=new QWidget(&w);
    wgt->resize(400,400);
    wgt->show();
    QPainter* painter = new QPainter();
    painter->begin(wgt);*/
QImage* img=new QImage("frame1.ppm");
/*
//QLabel l(&w,0)
QLabel myLab(&w,0);
    myLab.setPixmap(QPixmap::fromImage(img));
    myLab.resize(900,700);
    myLab.show();
painter->drawImage(0,0,img);
painter->end();*/

    char* filename_out = "log_.log";
    FILE * out;

    if((out=fopen(filename_out,"w")) >= 0 ) {
        fprintf(out,"Hello!\n");
        fclose(out);
        }
    //getchar();


    av_register_all();

    AVFormatContext *pFormatCtx=NULL;

    // Open video file
    char* filename_in = "face.mp4";

    //Look header info
    if(avformat_open_input(&pFormatCtx, filename_in, NULL, NULL)!=0)
         {printf("Couldn't open video file  \n");return -1;} // Couldn't open file

    // Retrieve stream information
    if(avformat_find_stream_info(pFormatCtx,NULL)<0)
      {printf("Couldn't find stream information\n");return -1;} // Couldn't find stream information

    // Dump information about file onto standard error
    av_dump_format(pFormatCtx, 0, filename_in, 0);


    //search video stream
    int i;
    AVCodecContext *pCodecCtx;

    // Find the first video stream
    int videoStream=-1;
    for(i=0; i<pFormatCtx->nb_streams; i++)
      if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
        videoStream=i;
        break;
      }
    if(videoStream==-1)
      return -1; // Didn't find a video stream

    // Get a pointer to the codec context for the video stream
    pCodecCtx=pFormatCtx->streams[videoStream]->codec;


    //Search codec
    AVCodec *pCodec;

    // Find the decoder for the video stream
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==NULL) {
      fprintf(stderr, "Unsupported codec!\n");
      return -1; // Codec not found
    }
    // Open codec
    if(avcodec_open(pCodecCtx, pCodec)<0)
      return -1; // Could not open codec

    //Store frame
    AVFrame *pFrame;

    // Allocate video frame
    pFrame=avcodec_alloc_frame();


    // Allocate an AVFrame structure
    AVFrame * pFrameRGB=avcodec_alloc_frame();
    if(pFrameRGB==NULL)
      return -1;

    uint8_t *buffer;
    int numBytes;
    // Determine required buffer size and allocate buffer
    numBytes=avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,
                                pCodecCtx->height);
    buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));


    // Assign appropriate parts of buffer to image planes in pFrameRGB
    // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
    // of AVPicture
    avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24,
                    pCodecCtx->width, pCodecCtx->height);


    SwsContext *pSWSContext = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, PIX_FMT_RGB24, SWS_BILINEAR, 0, 0, 0);

    int frameFinished;
    AVPacket packet;


    QImage* img1;

    i=0;
    while(av_read_frame(pFormatCtx, &packet)>=0)
    {
      // Is this a packet from the video stream?
      if(packet.stream_index==videoStream)
      {
            // Decode video frame
        avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished,
                             &packet);

        // Did we get a video frame?
        if(frameFinished)
        {
        // Convert the image from its native format to RGB

           /* img_convert((AVPicture *)pFrameRGB, PIX_FMT_RGB24,
                (AVPicture*)pFrame, pCodecCtx->pix_fmt,
                            pCodecCtx->width, pCodecCtx->height);
             sws_scale()
                     //pFrameRGB->l*/


             //Scale the raw data/convert it to our video buffer...
             sws_scale(pSWSContext, pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);
            // Save the frame to disk
            if(++i<=1)
            {
                unsigned char* ch;//=new unsigned char();//=(int*)pFrameRGB->data[100];
                //int* ch=new int();
                ch=pFrameRGB->data[0];
                printf("%c\n",*ch);
                char* imageOutName = "frame1_mat.ppm";
                Mat* image_mat=new Mat();//(pCodecCtx->height, pCodecCtx->width,pFrameRGB->linesize[0]*pCodecCtx->height, ch);
                image_mat->create(pCodecCtx->height,pCodecCtx->width,CV_8UC(3));
                for(int row=0; row<pCodecCtx->height;row++)
                {
                    for(int col=0; col<pCodecCtx->width; col++)
                    {
                        image_mat->at<Vec3b>(row,col)[0]=pFrameRGB->data[0][row*pFrameRGB->linesize[0]+col*3+2];
                        image_mat->at<Vec3b>(row,col)[1]=pFrameRGB->data[0][row*pFrameRGB->linesize[0]+col*3+1];
                        image_mat->at<Vec3b>(row,col)[2]=pFrameRGB->data[0][row*pFrameRGB->linesize[0]+col*3+0];
                    }

                }

              imwrite( imageOutName, *image_mat );
              SaveFrame(pFrameRGB, pCodecCtx->width,
                        pCodecCtx->height, i);
  /*               if(i==1)
                 {

                     //====================================

                     //if(!pFrameRGB->data) {
                     int imageWidth = pCodecCtx->width;
                     int imageHeight = pCodecCtx->height;
                     uchar* imageData = new uchar[4*imageWidth*imageHeight];
                     for(int i = 0; i < imageWidth*imageHeight; i++) {
                     imageData[i*4+3] = 0xFF;
                     }


                     int pixels = imageWidth * imageHeight;
                     uchar* src = (uchar*)(&pFrameRGB->data[0]);
                     uchar* srcEnd = src + (3*pixels);
                     uchar* dest = imageData;
*/
                     //do {
                     //memcpy(dest, src, 3);
                     /*int i=0,j=0;
                     do{
                     (*imageData)[i]=(*pFrameRGB->data)[j];
                     i++;j++;
                     *dest[i]=*src[j];
                     i++;j++;
                     *dest[i]=*src[j];
                     i++;j++;i++;
                     while(j<pixels*3)
                     //dest += 4;
                     //src += 3;*/
                     //} while(src < srcEnd);

                     //img1=new QImage(imageData,pCodecCtx->width,pCodecCtx->height,QImage::Format_RGB32);
            /*         printf("done\n");
                     printf("wight=%d,height=%d\n",pCodecCtx->height,pFrameRGB->linesize);
                    // }

                     //=====================================
                }*/
            }
        }
      }

      // Free the packet that was allocated by av_read_frame
      av_free_packet(&packet);
    }
    printf("Hello1!\n");
    QPushButton B1("Stop",&w);
    QObject::connect(&B1,SIGNAL(clicked()),&a,SLOT(quit()));
    B1.show();

    //if(!img1)
    //{
    /*    printf("ololo\n");
    QLabel myLab(&w,0);
    myLab.move(50,50);
    myLab.setPixmap(QPixmap::fromImage(*img1));
    myLab.resize(900,700);
    myLab.show();*/
    //}
    //img1->save("test.jpg","JPG");
    //QPainter painter(&w);
    //w->update();
    //QPainter* painter=new QPainter(&w);
    //painter->begin(&w);
    //w->update();
    //painter->drawImage(0,0,*img1);
    //w->update();
    //painter->end();
    //sleep(60);

    // Free the RGB image
    av_free(buffer);
    av_free(pFrameRGB);
    // Free the YUV frame
    av_free(pFrame);

    // Close the codec
    avcodec_close(pCodecCtx);

    // Close the video file
    av_close_input_file(pFormatCtx);

    //return i;

printf("end\n");
    return a.exec();
}


/*
// Protection against inclusion by a C++ file
#ifdef __cplusplus
extern "C" {
#endif

// C code
// ....

// Closing the protection against inclusion by a C++ file
#ifdef __cplusplus
}
#endif
#endif

  */
