#include "videoreader.h"

VideoReader::VideoReader(int size)
{
    frames.reserve(size);
    blured_frames.reserve(size);
}

VideoReader::VideoReader()
{
}


 VideoReader::~VideoReader()
{
}
int VideoReader::ReadFrames(const string& videofilename_in, int pyramid_level, int framesLimit)
{
    AVFormatContext *pFormatCtx=NULL;

    cout << videofilename_in << endl;

    if(avformat_open_input(&pFormatCtx, videofilename_in.c_str(), NULL, NULL)!=0)
    {
        printf("Couldn't open video file  \n");
        return -1;
    }

     // Retrieve stream information
    if(avformat_find_stream_info(pFormatCtx,NULL)<0)
    {
        printf("Couldn't find stream information\n");
        return -1;
    }

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
    if(avcodec_open2(pCodecCtx, pCodec, nullptr) < 0)
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
                if(++i<=framesLimit)
                {
                 //Mat* image_mat=new Mat();//(pCodecCtx->height, pCodecCtx->width,pFrameRGB->linesize[0]*pCodecCtx->height, ch);
                    Mat* image_mat = new Mat();
                    Mat* image_blured = new Mat();

                    frames.push_back(image_mat);
                    blured_frames.push_back(image_blured);

                    image_mat->create(pCodecCtx->height,pCodecCtx->width,CV_8UC3);
                    image_blured->create(pCodecCtx->height,pCodecCtx->width,CV_8UC3);

                    for(int row=0; row<pCodecCtx->height;row++)
                        for(int col=0; col<pCodecCtx->width; col++)
                        {
                            image_mat->at<Vec3b>(row,col)[0]=pFrameRGB->data[0][row*pFrameRGB->linesize[0]+col*3+2];
                            image_mat->at<Vec3b>(row,col)[1]=pFrameRGB->data[0][row*pFrameRGB->linesize[0]+col*3+1];
                            image_mat->at<Vec3b>(row,col)[2]=pFrameRGB->data[0][row*pFrameRGB->linesize[0]+col*3+0];

                            image_blured->at<Vec3b>(row,col)[0]=pFrameRGB->data[0][row*pFrameRGB->linesize[0]+col*3+2];
                            image_blured->at<Vec3b>(row,col)[1]=pFrameRGB->data[0][row*pFrameRGB->linesize[0]+col*3+1];
                            image_blured->at<Vec3b>(row,col)[2]=pFrameRGB->data[0][row*pFrameRGB->linesize[0]+col*3+0];
                        }
                    for(int lvl=1; lvl<=pyramid_level;lvl++)
                        pyrDown(*image_blured, *image_blured, Size(image_blured->cols/2, image_blured->rows/2));
                }
            }
        }

       // Free the packet that was allocated by av_read_frame
        av_free_packet(&packet);
    }


    NumberOfFrames = i - 6; //throw out 6 last frames
    blurred_frameHeight = blured_frames[0]->rows;
    blurred_frameWidth = blured_frames[0]->cols;
    frameHeight = frames[0]->rows;
    frameWidth = frames[0]->cols;

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

int VideoReader::PrintFrames(void) const
{
   if(this->frames[0]==nullptr)
   {
        cout << "Error! No frames detected.\n" << endl;
        return 1;
   }

   for(int i = 0; i < 2/*Numberofframes*/; i++)
   {
        const string frame_filename {"frame"+to_string(i)+".ppm"};
        imwrite(frame_filename.c_str(), *(this->frames[i]));
   }
   return 0;
}

int VideoReader::getFrameHeight(void) const
{
    return frameHeight;
}

int VideoReader::getFrameWidth(void) const
{
    return frameWidth;
}

int VideoReader::getNumberOfFrames(void) const
{
    return NumberOfFrames;
}

int VideoReader::getBlurredFrameHeight(void) const
{
    return blurred_frameHeight;
}

int VideoReader::getBlurredFrameWidth(void) const
{
    return blurred_frameWidth;
}

Mat** VideoReader::getFrames(void)
{
    return frames.data();
}

Mat** VideoReader::getBluredFrames(void)
{
    return blured_frames.data();
}


void print_frame_txt(Mat* frame, const string &filename)
{
    int fr_rows = frame->rows;
    int fr_cols = frame->cols;
    FILE * stream;
    if (fopen_s(&stream,filename.c_str(), "w") == 0)
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
