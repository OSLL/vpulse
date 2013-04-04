#ifndef VIDEOREADER_H
#define VIDEOREADER_H

#include "Conf.h"
#include "processor.h"
using namespace cv;
using namespace std;

class VideoReader
{
private:
    Mat* frames[FRAMES_MAX];
    Mat* blured_frames[FRAMES_MAX];
    int NumberOfFrames;
    int frameHeight;
    int frameWidth;

    int workstady;
    Mat* cvMatframe;
    int mode;
public:

    int ReadFrames(const char* videofilename_in, int pyramid_level);
    int PyrUpBlured(int pyramid_level);
    int PrintFrames(void);
    int AddPulseToFrames(void);

    int getFrameHeight(void);
    int getFrameWidth(void);
    int getNumberOfFrames(void);
    Mat** getFrames(void);
    Mat** getBluredFrames(void);

    void CVReadVideo(const char* videofilename_in);
    /*Mat**/ IplImage* cvframe_;
    void allocateMatBuffer(int n_height, int n_width);

    VideoReader();
    ~VideoReader();
};

#endif // VIDEOREADER_H
