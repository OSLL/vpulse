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

    int framecounter;
    int bufferflag;

    double* AllFrames;       //data
    double* buffer2;

    //fft_tmp_data-------------
    fftw_complex* out_fft;
    double* in_fft;
    fftw_plan p;
    fftw_complex* in_ifft;
    double* out_ifft;
    fftw_plan p_ifft;
    //-------------------------

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
    processor(/*, Mat** Frames*/);
    ~processor();
    void work(double fLow, double fHight, double ampFactor);
    void VectorToFrames(double* src, Mat** dst, int frWidth, int frHeight, int NofFrames);
    int AddPulseToFrames(Mat* frames,double *pulse, int NofFrames, int frame_number);
    double* getAllFrames(void);
    int getFrH(void);
    int getFrW(void);
    int getNFr(void);
    void NearInterpolation(double* src, double* dst, int oldwidth, int oldheight, int newwidth, int newheight, int nofFr, int frameInd);
    //tmp==================
    void PrintData(double* src, long len, const char* filename);
    void addFrame(Mat* frame);
    void init(int NumberOfFrames_in, int frameHeight_in, int frameWidth_in, int sRate_in);
    void IncFrCounter(void);
    int getFrCounter(void);
    void initFFT_IFFT(void);
    void clearFFT_IFFT(void);
    void changeBufferFlag(void);



    //=====================
};

#endif // PROCESSOR_H
