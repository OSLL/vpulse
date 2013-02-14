#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "Conf.h"
using namespace cv;

class processor
{
private:
    int NumberOfFrames;
    int frameHeight;
    int frameWidth;
    float samplingRate;

    float* AllFrames;       //data

    void FramesToVector(Mat** src, float* dst);
    void rgb2yiq(void);
    void normalize(void);
    int* createFreqMask(float fLow, float fHight);

public:
    processor(int NumberOfFrames_in, int frameHeight_in, int frameWidth_in, int sRate_in, Mat** Frames);
    ~processor();
    void work(float fLow, float fHight);
    float* getAllFrames(void);
    int getFrH(void);
    int getFrW(void);
    int getNFr(void);
    //tmp==================
    void PrintData(float* src, long len, const char* filename);
    //=====================
};

#endif // PROCESSOR_H
