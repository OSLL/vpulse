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
public:

    int ReadFrames(const char* videofilename_in, int pyramid_level);
    int PrintFrames(void);

    VideoReader();
    ~VideoReader();
};

#endif // VIDEOREADER_H
