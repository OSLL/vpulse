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
    int samplingRate;

    double* AllFrames;       //data

    void FramesToVector(Mat** src, double* dst);
    void rgb2yiq(void);
    void normalize(double* src, long len, double factor);
    int* createFreqMask(double fLow, double fHight);
    void copyVector(double* src, double* dst, long len);
    void copyFFTW_cpx(fftw_complex* src, fftw_complex* dst, long len);

public:
    processor(int NumberOfFrames_in, int frameHeight_in, int frameWidth_in, int sRate_in, Mat** Frames);
    ~processor();
    void work(double fLow, double fHight);
    double* getAllFrames(void);
    int getFrH(void);
    int getFrW(void);
    int getNFr(void);
    //tmp==================
    void PrintData(double* src, long len, const char* filename);
    //=====================
};

#endif // PROCESSOR_H
