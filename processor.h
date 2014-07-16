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
    int debugflag;

    double* AllFrames;       //data

    void FramesToVector(Mat** src, double* dst, int frWidth, int frHeight, int NofFrames);

    void rgb2yiq(double* srcDst, int frWidth, int frHeight, int NofFrames);
    void yiq2rgb(double* srcDst, int frWidth, int frHeight, int NofFrames);
    void normalize(double* src, long len, double factor);
    int* createFreqMask(double fLow, double fHight);
    void copyVector(double* src, double* dst, long len);
    void copyFFTW_cpx(fftw_complex* src, fftw_complex* dst, long len);
    void applyMask(fftw_complex*src, fftw_complex* dst, int* mask, long len);
    void sumVector(double* src1, double *src2, double* dst, long len);
    void rgbBoarder(double* src, long len);
    void YIQ2RGBnormalizeColorChannels(double* srcDst, int frWidth, int frHeight, int NofFrames);

public:
    processor(int NumberOfFrames_in, int frameHeight_in, int frameWidth_in, int sRate_in, Mat** Frames);
    ~processor();
    void work(double fLow, double fHight, double ampFactor);
    void VectorToFrames(double* src, Mat** dst, int frWidth, int frHeight, int NofFrames);
    int AddPulseToFrames(Mat** frames/*, Mat** pulse*/, double* result, int NofFrames);
    double* getAllFrames(void);
    std::vector<double>& receive_pixel_values(int i, int j) const;
    int getFrH(void) const;
    int getFrW(void) const;
    int getNFr(void) const;
    void NearInterpolation(double* src, double* dst, int oldwidth, int oldheight, int newwidth, int newheight, int nofFr, int frameInd);
    //tmp==================
    void PrintData(double* src, long len, const char* filename);
    //=====================
};

#endif // PROCESSOR_H
