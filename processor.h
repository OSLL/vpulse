#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "Conf.h"

using transRgbYiqMatr = double[9];

const transRgbYiqMatr rgb2yiqCoef {0.2989, 0.587, 0.144, 0.5959, -0.2744, -0.3216, 0.2115, -0.5229, 0.3114};
const transRgbYiqMatr yiq2rgbCoef {1.000, 0.956, 0.621, 1.000, -0.272, -0.647, 1.000, -1.106, 1.703};

const int channels {3};

class processor
{
private:
    int NumberOfFrames;
    int frameHeight;
    int frameWidth;
    int samplingRate;

    vector<double> AllFrames;       //data

    int calc_pixel_coor(int k, int row, int col, int channel) const;

public:
    vector<int> createFreqMask(double fLow, double fHigh) const;
    processor(int NumberOfFrames_in, int sRate_in, vector<std::unique_ptr<Mat> > &Frames);
    void amplify(double fLow, double fHight, double ampFactor);
    vector<double> receive_pixel_values(int row, int col, int channel) const;
    void insert_pixel_values(const vector<double>& values, int row, int col, int channel);
    int getFrH() const;
    int getFrW() const;
    int getNFr() const;
    int AddPulseToFrames(vector<unique_ptr<Mat>>& frames, int NofFrames) const;
    double at(int k, int row, int col, int channel) const;
};

void YIQ2RGBnormalizeColorChannels(vector<double>& srcDst, int frWidth, int frHeight, int NofFrames);
void NearInterpolation(const vector<double>& src, vector<double>& dst, int oldwidth, int oldheight, int newwidth, int newheight, int frameInd);
void FramesToVector(vector<unique_ptr<Mat> > &src, vector<double>& dst);
void FramesToVector(unique_ptr<Mat>& src, vector<double>& dst);
void VectorToFrames(const vector<double> &src, vector<unique_ptr<Mat>>& dst, int frWidth, int frHeight);
void VectorToFrames(const vector<double>& src, unique_ptr<Mat>& dst, int frWidth, int frHeight);
complex_vector applyMask(const complex_vector& src, const vector<int>& mask);


#endif // PROCESSOR_H
