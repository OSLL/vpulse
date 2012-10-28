#include <QtGui/QApplication>
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
    char* filename_in = "test.mkv";

    //Look header info
    if(avformat_open_input(&pFormatCtx, filename_in, NULL, NULL)!=0)
         {printf("Couldn't open file vidmag.mov \n");return -1;} // Couldn't open file

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
            if(++i<=30)
              SaveFrame(pFrameRGB, pCodecCtx->width,
                        pCodecCtx->height, i);
        }
      }

      // Free the packet that was allocated by av_read_frame
      av_free_packet(&packet);
    }
    // Free the RGB image
    av_free(buffer);
    av_free(pFrameRGB);
    // Free the YUV frame
    av_free(pFrame);

    // Close the codec
    avcodec_close(pCodecCtx);

    // Close the video file
    av_close_input_file(pFormatCtx);

    return i;


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
