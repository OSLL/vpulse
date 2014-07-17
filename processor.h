#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "Conf.h"

using namespace cv;
using transRgbYiqMatr = double[9];

const transRgbYiqMatr rgb2yiqCoef {0.2989, 0.587, 0.144, 0.5959, -0.2744, -0.3216, 0.2115, -0.5229, 0.3114};
const transRgbYiqMatr yiq2rgbCoef {1.000, 0.956, 0.621, 1.000, -0.272, -0.647, 1.000, -1.106, 1.703};


class processor
{
private:
    const int channels = 3;
    int NumberOfFrames;
    int frameHeight;
    int frameWidth;
    int samplingRate;
    int debugflag;

    double* AllFrames;       //data

    void FramesToVector(Mat** src, double* dst, int frWidth, int frHeight, int NofFrames);

    void rgb2yiq(double* srcDst, int frWidth, int frHeight, int NofFrames, bool rev);
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
    std::vector<double> receive_pixel_values(int row, int col) const;
    int getFrH(void) const;
    int getFrW(void) const;
    int getNFr(void) const;
    double at(int k, int row, int col) const;
    void NearInterpolation(double* src, double* dst, int oldwidth, int oldheight, int newwidth, int newheight, int nofFr, int frameInd);
    //tmp==================
    void PrintData(double* src, long len, const char* filename);
    //=====================
};

#endif // PROCESSOR_H
