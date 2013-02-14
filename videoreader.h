#ifndef VIDEOREADER_H
#define VIDEOREADER_H

#include "Conf.h"
using namespace cv;
using namespace std;

class VideoReader
{
private:
    Mat* frames[FRAMES_MAX];
    int NumberOfFrames;
    int frameHeight;
    int frameWidth;
public:

    int ReadFrames(const char* videofilename_in, int pyramid_level);
    int PrintFrames(void);

    int getFrameHeight(void);
    int getFrameWidth(void);
    int getNumberOfFrames(void);
    Mat** getFrames(void);

    VideoReader();
    ~VideoReader();
};

#endif // VIDEOREADER_H
