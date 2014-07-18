#include "videoreader.h"

VideoReader::VideoReader()
{
    av_register_all();
    //this->frames=
    //template<typename _Tp> _Tp* allocate(size_t n)
    //template<Mat> Mat* allocate(100);
    //this->frames=cv::allocate<Mat>(300);
    //this->frames=cv::allocate<Mat(100,200,CV_8UC(3))>(300);
}

 VideoReader::~VideoReader()
{
    //template<typename _Tp> void deallocate(_Tp* ptr, size_t n)
     //template<Mat> void deallocate(this->frames,100);
     //Mat::allocator
     //cv::deallocate(this->frames,300);



}
int VideoReader::ReadFrames(const string& videofilename_in, int pyramid_level)
 {
     AVFormatContext *pFormatCtx=NULL;

     // Open video file
     //char* filename_in = "face.mp4";
    cout << videofilename_in << endl;
     //Look header info
     if(avformat_open_input(&pFormatCtx, videofilename_in.c_str(), NULL, NULL)!=0)
          {printf("Couldn't open video file  \n");return -1;} // Couldn't open file

     // Retrieve stream information
     if(avformat_find_stream_info(pFormatCtx,NULL)<0)
       {printf("Couldn't find stream information\n");return -1;} // Couldn't find stream information

     // Dump information about file onto standard error
     av_dump_format(pFormatCtx, 0, videofilename_in.c_str(), 0);


     //search video stream
     AVCodecContext *pCodecCtx;

     // Find the first video stream
     int videoStream=-1;
     for(unsigned int i = 0; i < pFormatCtx->nb_streams; i++)
       if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
       {
         videoStream=i;
         break;
       }
     if (videoStream==-1)
       return -1; // Didn't find a video stream

     // Get a pointer to the codec context for the video stream
     pCodecCtx=pFormatCtx->streams[videoStream]->codec;


     //Search codec
     AVCodec *pCodec;

     // Find the decoder for the video stream
     pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
     if(pCodec==NULL)
     {
       cout << "Unsupported codec!\n" << endl;
       return -1; // Codec not found
     }
     // Open codec
     if(avcodec_open2(pCodecCtx, pCodec, nullptr)<0)
       return -1; // Could not open codec

     // Allocate video frame
     AVFrame* pFrame = avcodec_alloc_frame();

     // Allocate an AVFrame structure
     AVFrame* pFrameRGB = avcodec_alloc_frame();
     if(pFrameRGB==NULL)
       return -1;


     // Determine required buffer size and allocate buffer
     int numBytes {avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,pCodecCtx->height)};
     uint8_t *buffer = (uint8_t*)av_malloc(sizeof(uint8_t)*numBytes);


     // Assign appropriate parts of buffer to image planes in pFrameRGB
     // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
     // of AVPicture
     avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24,pCodecCtx->width, pCodecCtx->height);

     //this->frameHeight=pCodecCtx->height;
     //this->frameWidth=pCodecCtx->width;

     SwsContext *pSWSContext = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, PIX_FMT_RGB24, SWS_BILINEAR, 0, 0, 0);

     int frameFinished;
     AVPacket packet;

     int i = 0;
     while(av_read_frame(pFormatCtx, &packet)>=0)
     {
       // Is this a packet from the video stream?
       if(packet.stream_index==videoStream)
       {
             // Decode video frame
         avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

         // Did we get a video frame?
         if(frameFinished)
         {
              //Scale the raw data/convert it to our video buffer.
             sws_scale(pSWSContext, pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);
             // Save the frame to disk
             if(++i<=FRAMES_MAX)
             {
                 //Mat* image_mat=new Mat();//(pCodecCtx->height, pCodecCtx->width,pFrameRGB->linesize[0]*pCodecCtx->height, ch);
                 this->frames[i-1]=new Mat();
                 this->blured_frames[i-1]= new Mat();
                 Mat* image_mat =this->frames[i-1];
                 Mat* image_blured = this->blured_frames[i-1];

                 image_mat->create(pCodecCtx->height,pCodecCtx->width,CV_8UC3);
                 image_blured->create(pCodecCtx->height,pCodecCtx->width,CV_8UC3);
                 for(int row=0; row<pCodecCtx->height;row++)
                 {
                     for(int col=0; col<pCodecCtx->width; col++)
                     {
                         image_mat->at<Vec3b>(row,col)[0]=pFrameRGB->data[0][row*pFrameRGB->linesize[0]+col*3+2];
                         image_mat->at<Vec3b>(row,col)[1]=pFrameRGB->data[0][row*pFrameRGB->linesize[0]+col*3+1];
                         image_mat->at<Vec3b>(row,col)[2]=pFrameRGB->data[0][row*pFrameRGB->linesize[0]+col*3+0];

                         image_blured->at<Vec3b>(row,col)[0]=pFrameRGB->data[0][row*pFrameRGB->linesize[0]+col*3+2];
                         image_blured->at<Vec3b>(row,col)[1]=pFrameRGB->data[0][row*pFrameRGB->linesize[0]+col*3+1];
                         image_blured->at<Vec3b>(row,col)[2]=pFrameRGB->data[0][row*pFrameRGB->linesize[0]+col*3+0];
                     }

                 }

                for(int lvl=1; lvl<=pyramid_level;lvl++)
                    pyrDown(*image_blured, *image_blured, Size(image_blured->cols/2, image_blured->rows/2));
             }
         }
       }

       // Free the packet that was allocated by av_read_frame
       av_free_packet(&packet);
     }


     this->NumberOfFrames=i-6; //throw out 6 last frames
     PyrUpBlured(pyramid_level);
     this->frameHeight=this->blured_frames[0]->rows;
     this->frameWidth=this->blured_frames[0]->cols;

     av_free(buffer);
     av_free(pFrameRGB);
     // Free the YUV frame
     av_free(pFrame);

     // Close the codec
     avcodec_close(pCodecCtx);

     // Close the video file
     avformat_close_input(&pFormatCtx);

     return 0;
}

void print_frame_txt(Mat* frame,const char* filename)
{
    int fr_rows = frame->rows;
    int fr_cols = frame->cols;
    FILE * stream;
            if ((stream=fopen(filename, "w")) != 0)
    {
        fprintf(stream, "row\t\t R \t\t\t G\t\t\tB\t\t\n");
        for(int curr_row = 0; curr_row < fr_rows; curr_row++)
        {
            fprintf(stream, "\n \t row = %d\n",curr_row);
            for(int curr_col = 0; curr_col <fr_cols; curr_col++)
            {
                fprintf(stream, "%d %u \t|\t %u \t|\t %u",curr_row, frame->at<Vec3b>(curr_row,curr_col)[0],frame->at<Vec3b>(curr_row,curr_col)[1],frame->at<Vec3b>(curr_row,curr_col)[2]);
                fputc('\n',stream);
            }

        }
    }
    fclose(stream);
}

int VideoReader::PrintFrames(void)
{
   if(this->frames[0]==nullptr)
    {
       cout << "Error! No frames detected.\n" << endl;
       return 1;
    }
    else
    {
        for(int i=0; i<NumberOfFrames; i++)
        {
            const string frame_filename {"frame"+to_string(i)+".ppm"};
            imwrite(frame_filename.c_str(), *(this->frames[i]));
        }
    }
 return 0;
}

int VideoReader::PyrUpBlured(int pyramid_level)
{
    if(pyramid_level>=1)
    {
        for(int NofFr = 0; NofFr < this->NumberOfFrames; NofFr++)
        {
            Mat* image_mat=this->blured_frames[NofFr];
            for(int lvl=1; lvl<=pyramid_level;lvl++)
                resize(*image_mat,*image_mat,Size(image_mat->cols*2, image_mat->rows*2),0,0,INTER_CUBIC);
        }
    }
    return 0;
}

int VideoReader::AddPulseToFrames(void)
{
    for(int NofFr = 0; NofFr < NumberOfFrames; NofFr++)
    {
        Mat* image_mat = frames[NofFr];
        Mat* image_blured = blured_frames[NofFr];
        for(int row = 0; row < frames[0]->rows; row++)
            for(int col = 0; col < frames[0]->cols; col++)
            {
                image_mat->at<Vec3b>(row,col)[0] += image_blured->at<Vec3b>(row,col)[0];
                image_mat->at<Vec3b>(row,col)[1] += image_blured->at<Vec3b>(row,col)[1];
                image_mat->at<Vec3b>(row,col)[2] += image_blured->at<Vec3b>(row,col)[2];
            }
    }
    return 0;
}

int VideoReader::getFrameHeight(void)
{
    return(this->frameHeight);
}

int VideoReader::getFrameWidth(void)
{
    return(this->frameWidth);
}

int VideoReader::getNumberOfFrames(void)
{
    return(this->NumberOfFrames);
}

Mat** VideoReader::getFrames(void)
{
    return(this->frames);
}

Mat** VideoReader::getBluredFrames(void)
{
    return(this->blured_frames);
}
