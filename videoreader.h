#ifndef VIDEOREADER_H
#define VIDEOREADER_H

#include "Conf.h"
using namespace cv;
using namespace std;

class VideoReader
{
private:
    vector<Mat*> frames;
    vector<Mat*> blured_frames;
    int NumberOfFrames;
    int frameHeight;
    int frameWidth;
    int blurred_frameHeight;
    int blurred_frameWidth;
public:

    int ReadFrames(const string& videofilename_in, int pyramid_level, int framesLimit);
    int PrintFrames(void) const;

    int getFrameHeight(void) const;
    int getFrameWidth(void) const;
    int getBlurredFrameHeight(void) const;
    int getNumberOfFrames(void) const;
    int getBlurredFrameWidth(void) const;

    Mat** getFrames(void);
    Mat** getBluredFrames(void);

    VideoReader(int size);
    VideoReader();
    ~VideoReader();
};

void print_frame_txt(Mat* frame,const string& filename);

#endif // VIDEOREADER_H
