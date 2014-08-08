#ifndef VIDEOREADER_H
#define VIDEOREADER_H

#include "Conf.h"
using namespace std;

class VideoReader
{
private:
    vector<unique_ptr<Mat>> frames;
    vector<unique_ptr<Mat>> blured_frames;
    int NumberOfFrames;
    int frameHeight;
    int frameWidth;
    int blurred_frameHeight;
    int blurred_frameWidth;
public:

    int ReadFrames(const string& videofilename_in, int pyramid_level, int framesLimit);

    int getFrameHeight(void) const;
    int getFrameWidth(void) const;
    int getBlurredFrameHeight(void) const;
    int getNumberOfFrames(void) const;
    int getBlurredFrameWidth(void) const;

    vector<unique_ptr<Mat>>& getFrames(void);
    vector<unique_ptr<Mat>>& getBluredFrames(void);
};

#endif // VIDEOREADER_H
