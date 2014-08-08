#include "videoreader.h"

void pyrDown(Mat& img)
{
    const int mask[5][5] =
    {{1, 4, 6, 4, 1},
     {4, 16, 24, 16, 4},
     {6, 24, 36, 24, 6},
     {4, 16, 24, 16, 4},
     {1, 4, 6, 4, 1}};
     Mat tmp(img.get_rows() - 4,img.get_cols() - 4,img.get_channels());
     for(int i = 2; i < img.get_rows() - 2; i++)
        for(int j = 2; j < img.get_cols() - 2; j++)
            for(int ch = 0; ch < img.get_channels(); ch++)
            {
                double sum = 0;
                for(int di = -2; di <= 2; di++)
                    for(int dj = -2; dj <= 2; dj++)
                        sum += img.at(i+di,j+dj,ch) * mask[di+2][dj+2];
                sum /= (double)256.0;
                tmp.at(i-2,j-2,ch) = sum;
            }
    Mat res((tmp.get_rows()+1)/2,(tmp.get_cols()+1)/2,tmp.get_channels());

    for(int i = 0; i < tmp.get_rows(); i += 2)
        for(int j = 0; j < tmp.get_cols(); j += 2)
            for(int ch = 0; ch < tmp.get_channels(); ch++)
                res.at(i/2,j/2,ch) = tmp.at(i,j,ch);
    img = move(res);
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
    AVFrame* pFrame = av_frame_alloc();

     // Allocate an AVFrame structure
    AVFrame* pFrameRGB = av_frame_alloc();
    if(pFrameRGB==NULL)
        return -1;


     // Determine required buffer size and allocate buffer
    int numBytes {avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,pCodecCtx->height)};
    uint8_t *buffer = (uint8_t*)av_malloc(sizeof(uint8_t)*numBytes);


     // Assign appropriate parts of buffer to image planes in pFrameRGB
     // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
     // of AVPicture
    avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24,pCodecCtx->width, pCodecCtx->height);

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
                if(i<=framesLimit)
                {
                    i++;
                    auto image_mat = unique_ptr<Mat>(new Mat(pCodecCtx->height,pCodecCtx->width,3));

                    for(int row=0; row<pCodecCtx->height;row++)
                        for(int col=0; col<pCodecCtx->width; col++)
                        {
                            image_mat->getVec(row,col)[0]=pFrameRGB->data[0][row*pFrameRGB->linesize[0]+col*3+2];
                            image_mat->getVec(row,col)[1]=pFrameRGB->data[0][row*pFrameRGB->linesize[0]+col*3+1];
                            image_mat->getVec(row,col)[2]=pFrameRGB->data[0][row*pFrameRGB->linesize[0]+col*3+0];
                        }

                    unique_ptr<Mat> image_blured(new Mat(*image_mat));
                    for(int lvl=1; lvl<=pyramid_level;lvl++)
                        pyrDown(*image_blured);

                    frames.push_back(move(image_mat));
                    blured_frames.push_back(move(image_blured)); //image_mat and image_blurred are no longer valid
                }
            }
        }

       // Free the packet that was allocated by av_read_frame
        av_free_packet(&packet);
    }


    NumberOfFrames = i;
    blurred_frameHeight = blured_frames[0]->get_rows();
    blurred_frameWidth = blured_frames[0]->get_cols();
    frameHeight = frames[0]->get_rows();
    frameWidth = frames[0]->get_cols();

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

vector<unique_ptr<Mat>>& VideoReader::getFrames(void)
{
    return frames;
}

vector<unique_ptr<Mat>>& VideoReader::getBluredFrames(void)
{
    return blured_frames;
}
