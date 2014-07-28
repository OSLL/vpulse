#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "Conf.h"

using namespace cv;
using transRgbYiqMatr = double[9];

const transRgbYiqMatr rgb2yiqCoef {0.2989, 0.587, 0.144, 0.5959, -0.2744, -0.3216, 0.2115, -0.5229, 0.3114};
const transRgbYiqMatr yiq2rgbCoef {1.000, 0.956, 0.621, 1.000, -0.272, -0.647, 1.000, -1.106, 1.703};

const int channels = 3;

class processor
{
private:
    int NumberOfFrames;
    int frameHeight;
    int frameWidth;
    int samplingRate;

    vector<double> AllFrames;       //data

    vector<int> createFreqMask(double fLow, double fHigh);
    int calc_pixel_coor(int k, int row, int col, int channel) const;

public:
    processor(int NumberOfFrames_in, int frameHeight_in, int frameWidth_in, int sRate_in, Mat** Frames);
    ~processor();
    void work(double fLow, double fHight, double ampFactor);
    vector<double> getAllFrames() const;
    vector<double> receive_pixel_values(int row, int col, int channel) const;
    void insert_pixel_values(const vector<double>& values, int row, int col, int channel);
    int getFrH() const;
    int getFrW() const;
    int getNFr() const;
    int AddPulseToFrames(Mat** frames, int NofFrames) const;
    double at(int k, int row, int col, int channel) const;
};

void YIQ2RGBnormalizeColorChannels(vector<double>& srcDst, int frWidth, int frHeight, int NofFrames);
void NearInterpolation(const vector<double>& src, vector<double>& dst, int oldwidth, int oldheight, int newwidth, int newheight, int frameInd);
void FramesToVector(Mat** src, const vector<double>& dst, int frWidth, int frHeight, int NofFrames);

#endif // PROCESSOR_H
