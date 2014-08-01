#include "processor.h"
using namespace std;

#include<functional>
#include<cmath>

void FramesToVector(Mat** src, vector<double>& dst, int NofFrames)
{
    int frHeight = src[0]->rows;
    int frWidth = src[0]->cols;

    for(int k = 0; k < NofFrames; k++)
        for(int row = 0; row < frHeight; row++)
            for(int col = 0; col < frWidth; col++)
            {
                Vec3b source = src[k]->at<Vec3b>(row,col);
                int destination_index {channels*(frWidth*frHeight*k+frWidth*row+col)};
                dst[destination_index + 0] = source[0];
                dst[destination_index + 1] = source[1];
                dst[destination_index + 2] = source[2];
            }
}


processor::processor(int NumberOfFrames_in, int sRate_in, Mat** Frames):
    NumberOfFrames(NumberOfFrames_in),
    frameHeight(Frames[0]->rows),
    frameWidth(Frames[0]->cols),
    samplingRate(sRate_in),
    AllFrames(NumberOfFrames*frameHeight*frameWidth*channels)
{
    FramesToVector(Frames, AllFrames, NumberOfFrames);
}

void VectorToFrames(const vector<double>& src, Mat** dst, int frWidth, int frHeight, int NofFrames)
{
    for(int k = 0; k < NofFrames; k++)
        for(int row = 0; row < frHeight; row++)
            for(int col = 0; col < frWidth; col++)
            {
                Vec3b* destination = &(dst[k]->at<Vec3b>(row,col));
                int source_index {channels*(frWidth*frHeight*k+frWidth*row+col)};
                (*destination)[0] = src[source_index + 0];
                (*destination)[1] = src[source_index + 1];
                (*destination)[2] = src[source_index + 2];
            }
}


void rgb2yiq(vector<double>& srcDst, int frWidth, int frHeight, int NofFrames, bool rev)
{
    int pixels = NofFrames*frHeight*frWidth*channels;
    const double* Coefs = rev ? &yiq2rgbCoef[0] : &rgb2yiqCoef[0];
    for(int i = 0; i < pixels; i+=3)
    {
       double tmp[3] {srcDst[i], srcDst[i + 1], srcDst[i + 2]};  //ugly code, could be done in one line using linear algebra
       srcDst[i + 0] = tmp[0]*Coefs[0] + tmp[1]*Coefs[1] + tmp[2]*Coefs[2];
       srcDst[i + 1] = tmp[0]*Coefs[3] + tmp[1]*Coefs[4] + tmp[2]*Coefs[5];
       srcDst[i + 2] = tmp[0]*Coefs[6] + tmp[1]*Coefs[7] + tmp[2]*Coefs[8];
    }
}

void normalize(vector<double>& src, double factor)
{
    for(auto& e : src)
        e/=factor;
}

vector<int> processor::createFreqMask(double fLow, double fHigh) const
{
    vector<int> indices;

    for(int i = 0; i < NumberOfFrames; i++)
    {
        double mask = (double)i/NumberOfFrames*samplingRate;
        if (mask > fLow && mask < fHigh)
            indices.push_back(i);
    }
    return indices;
}

complex_vector applyMask(const complex_vector& src, const vector<int>& mask)
{
    complex_vector dst(src.size(), complex_number(0,0));

    for(auto i : mask) //copy src data in elements with indices from mask
    {
        dst[i] = src[i];
    }
    return dst;
}

void processor::amplify(double fLow, double fHight, double ampFactor)
{
    normalize(AllFrames,255);

    rgb2yiq(AllFrames, frameWidth, frameHeight, NumberOfFrames, false);

    vector<int> mask = createFreqMask(fLow,fHight);

    complex_vector out_fft(NumberOfFrames);
    vector<double> in_fft(NumberOfFrames);

    fftw_plan p = fftw_plan_dft_r2c_1d((int)in_fft.size(), in_fft.data(), reinterpret_cast<fftw_complex*>(out_fft.data()), FFTW_ESTIMATE);

    complex_vector in_ifft(NumberOfFrames);
    vector<double> out_ifft(NumberOfFrames);

    fftw_plan p_ifft = fftw_plan_dft_c2r_1d(NumberOfFrames, reinterpret_cast<fftw_complex*>(in_ifft.data()), out_ifft.data(), FFTW_ESTIMATE);

    for(int row = 0; row < frameHeight; row++)
        for(int col = 0; col < frameWidth; col++)
            for(int channel = 0; channel < channels; channel++)
            {
                vector<double> tmp = receive_pixel_values(row,col,channel);
                in_fft.assign(tmp.begin(),tmp.end());
                fftw_execute(p);

                complex_vector tmp1 = applyMask(out_fft,mask);
                in_ifft.assign(tmp1.begin(),tmp1.end());
                fftw_execute(p_ifft);

                normalize(out_ifft,(double)NumberOfFrames/ampFactor);
                insert_pixel_values(out_ifft,row,col,channel);
            }
    fftw_destroy_plan(p);
    fftw_destroy_plan(p_ifft);
}

void YIQ2RGBnormalizeColorChannels(vector<double>& srcDst, int frWidth, int frHeight, int NofFrames)
{
    rgb2yiq(srcDst,frWidth,frHeight,NofFrames,true);

    int pixels = NofFrames*frHeight*frWidth*channels;
    for(int i = 0; i < pixels; i+=3)
    {
        double normfactor {max({srcDst[i + 0], srcDst[i + 1], srcDst[i + 2], 1.0})};

       if (normfactor > 1.0)
       {
            srcDst[i + 0]/=normfactor;
            srcDst[i + 1]/=normfactor;
            srcDst[i + 2]/=normfactor;
       }
    }
}

int processor::AddPulseToFrames(Mat** frames, int NofFrames) const
{
    const int FrWidth {frames[0]->cols};
    const int FrHeight {frames[0]->rows};
    const int FrChannels {frames[0]->channels()};
    const int frame_size {FrHeight*FrWidth*FrChannels};
    vector<double> fullFrame(frame_size);
    vector<double> pulseFrame(frame_size);

    for(int frame_number = 0; frame_number < NofFrames; frame_number++)
    {
        FramesToVector(&frames[frame_number], fullFrame, 1);
        normalize(fullFrame,255.0);
        rgb2yiq(fullFrame, FrHeight, FrWidth, 1, false);
        NearInterpolation(AllFrames,pulseFrame,frameWidth,frameHeight,FrWidth,FrHeight,
                          frame_number);
        std::transform(fullFrame.begin(), fullFrame.end(), pulseFrame.begin(), fullFrame.begin(),
                       std::plus<double>());                                                    //summing the vectors
        YIQ2RGBnormalizeColorChannels(fullFrame, FrWidth, FrHeight, 1);
        std::transform(fullFrame.begin(),fullFrame.end(),fullFrame.begin(),
                       [](double a){if (a > 1) return 1.0; if (a < 0) return 0.0; return a;});
        normalize(fullFrame,1.0/255);
        VectorToFrames(fullFrame, &frames[frame_number], FrWidth, FrHeight, 1);
    }
    return 0;
}

int processor::getFrH(void) const
{
    return frameHeight;
}

int processor::getFrW(void) const
{
    return frameWidth;
}

int processor::getNFr(void) const
{
    return NumberOfFrames;
}

void NearInterpolation(const vector<double>& src, vector<double>& dst, int oldwidth, int oldheight, int newwidth, int newheight, int frameInd)
{
   for(int cx = 0; cx < newwidth; cx++)
        for(int cy = 0; cy < newheight; cy++)
        {
            int oldcx = static_cast<int>(floor((double)cx / newwidth * oldwidth));
            int oldcy = static_cast<int>(floor((double)cy / newheight * oldheight));
            int nearest_match {channels*(oldwidth*oldheight*frameInd+oldwidth*oldcy+oldcx)};
            int pixel_index{channels * (cy * newwidth + cx)};
            dst[pixel_index + 0] = src[nearest_match + 0];
            dst[pixel_index + 1] = src[nearest_match + 1];
            dst[pixel_index + 2] = src[nearest_match + 2];
        }
}

vector<double> processor::receive_pixel_values(int row, int col, int channel) const
{
    vector<double> res(NumberOfFrames);

    for(int k = 0; k < (int)res.size(); k++)
        res[k] = AllFrames[calc_pixel_coor(k,row,col,channel)];

    return res;
}


void processor::insert_pixel_values(const vector<double>& values, int row, int col, int channel)
{
    if (values.size() != NumberOfFrames)
        throw Exception();
    for(int k = 0; k < (int)values.size(); k++)
        AllFrames[calc_pixel_coor(k,row,col,channel)] = values[k];

}


int processor::calc_pixel_coor(int k, int row, int col, int channel) const
{
    int width = frameWidth;
    int height = frameHeight;
    return channels*(width*height*k+width*row+col)+channel;
}


double processor::at(int k, int row, int col, int channel) const
{
    return AllFrames[calc_pixel_coor(k,row,col,channel)];
}
