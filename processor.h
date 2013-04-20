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
    double samplingRate;
    int debugflag;

    double* AllFrames;       //data

    //fft_header
    fftw_complex* out_fft;
    double* in_fft;
    fftw_plan p;
    //ifft_header
    fftw_complex* in_ifft;
    double* out_ifft;
    fftw_plan p_ifft;
    //frequency_mask
    int* mask;
    double* real_mask_freq;
    //rendering buffers:
    double* fullFrames;
    double* pulseFrames;
    double* pulseFrames_gauss;



public:
    void rgb2yiq(double* srcDst, int frWidth, int frHeight, int NofFrames);
    void yiq2rgb(double* srcDst, int frWidth, int frHeight, int NofFrames);
    void normalize(double* src, long len, double factor);
    int* createFreqMask(double fLow, double fHight);
    void copyVector(double* src, double* dst, long len);
    void copyFFTW_cpx(fftw_complex* src, fftw_complex* dst, long len);
    void applyMask(fftw_complex*src, fftw_complex* dst, int* mask, long len,int* teoretical_rate_ind);
    void sumVector(double* src1, double *src2, double* dst, long len);
    void rgbBoarder(double* src, long len);
    void rgbBoarder2(double* src, long len);
    void YIQ2RGBnormalizeColorChannels(double* srcDst, int frWidth, int frHeight, int NofFrames);
    void YIQ2RGBnormalizeColorChannels2(double* srcDst, int frWidth, int frHeight, int NofFrames);
    void InitFFT_IFFT_createFrMask(double fLow, double fHight);
    void ClearFFT_IFFT_mask(void);
    void AllocRendBuff(long lengthAll, long NofFr_);
    void freeRendBuff(void);
    void countPulse(double* result, double ampFact);
    double* getRealMaskFr(void);
    processor(int NumberOfFrames_in, int frameHeight_in, int frameWidth_in, double sRate_in, Mat** Frames);
    ~processor();
    processor();
    void init(int NumberOfFrames_in, int frameHeight_in, int frameWidth_in, double sRate_in/*, Mat** Frames*/);
    void FrameToVector(Mat** src, double* dst, int frWidth, int frHeight, int NofFrames, int frN);

    void work(double fLow, double fHight, double ampFactor);
    void VectorToFrames(double* src, Mat** dst, int frWidth, int frHeight, int NofFrames);
    void FramesToVector(Mat** src, double* dst, int frWidth, int frHeight, int NofFrames);
    int AddPulseToFrames(Mat** frames/*, Mat** pulse*/, double* result, int NofFrames);
    int AddPulseToFrames2(Mat** frames/*, Mat** pulse*/, double* result, int NofFrames);
    double* getAllFrames(void);
    int getFrH(void);
    int getFrW(void);
    int getNFr(void);
    double getFPS(void);
    double* get_pulse_frames(void);
    void sumVector_frame_arr(double* src1, double *src2, double* dst, int frH, int frW, int NofFr_,int frameNumber);
    void setFPS(double val);
    void NearInterpolation(double* src, double* dst, int oldwidth, int oldheight, int newwidth, int newheight, int nofFr, int frameInd);
    void NearInterpolation2(double* src, double* dst, int oldwidth, int oldheight, int newwidth, int newheight, int nofFr,int frameInd);
    //tmp==================
    void PrintData(double* src, long len, const char* filename);
    void countPulseRate(double* res);
    //=====================
    fftw_complex* get_out_fft(void);
    fftw_complex* get_in_ifft(void);
    double* get_in_fft(void);
    double* get_out_ifft(void);
    fftw_plan* get_fft_plan(void);
    fftw_plan* get_ifft_plan(void);
    int* get_mask(void);
    void render(Mat* frames,long LengthAll, int FrHeight, int FrWidth, int NofFrames, int frame_number);
void  gauss_3_3(double* src, double* dst, int width, int height , int nofFr, int frameInd);
};

#endif // PROCESSOR_H
