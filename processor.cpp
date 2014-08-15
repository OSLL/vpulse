#include "Processor.h"

using namespace std;

#include<functional>
#include<cmath>

namespace ProcessorUtilities
{

void FramesToVector(const vector<unique_ptr<Mat>>& src, vector<double>& dst)
{
    const size_t image_size {src[0]->size()};
    for(size_t i = 0; i < src.size(); i++)
        copy(src[i]->data().begin(), src[i]->data().end(), dst.begin()+ i*image_size);

}

void FramesToVector(const unique_ptr<Mat>& src, vector<double>& dst)
{
    dst = src->data();
}


void VectorToFrames(const vector<double>& src, unique_ptr<Mat>& dst)
{
    dst->data() = src;
}

void normalize(vector<double>& src, double factor)
{
    for(auto& e : src)
        e/=factor;
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

void YIQ2RGBnormalizeColorChannels(vector<double>& srcDst, int frWidth, int frHeight, int NofFrames)
{
    rgb2yiq(srcDst,frWidth,frHeight,NofFrames,true);

    int pixels = NofFrames*frHeight*frWidth*channels;
    for(int i = 0; i < pixels; i+=3)
    {
        double normfactor {max({srcDst[i + 0], srcDst[i + 1], srcDst[i + 2], 1.0})};

       if (normfactor > 1.0)
       {
            for(int j = 0; j < channels; j++)
                srcDst[i + j]/=normfactor;
       }
    }
}

void NearInterpolation(const vector<double>& src, vector<double>& dst, int oldwidth, int oldheight, int newwidth, int newheight, int frameInd)
{
    const int scaleRateW = newwidth * oldwidth;
    const int scaleRateH = newheight * oldheight;
    for(int cx = 0; cx < newwidth; cx++)
        for(int cy = 0; cy < newheight; cy++)
        {
            int oldcx = static_cast<int>(floor((double)cx / scaleRateW));
            int oldcy = static_cast<int>(floor((double)cy / scaleRateH));
            int nearest_match {channels*(oldwidth*oldheight*frameInd+oldwidth*oldcy+oldcx)};
            int pixel_index {channels * (cy * newwidth + cx)};
            for(int i = 0; i < channels; i++)
                dst[pixel_index + i] = src[nearest_match + i];
        }
}

}

using namespace ProcessorUtilities;

Processor::Processor(unsigned int sRate_in, const vector<unique_ptr<Mat> > &Frames):
    m_numberOfFrames(Frames.size()),
    m_frameHeight(Frames[0]->getRows()),
    m_frameWidth(Frames[0]->getCols()),
    m_samplingRate(sRate_in),
    m_AllFrames(m_numberOfFrames*m_frameHeight*m_frameWidth*channels)
{
    FramesToVector(Frames, m_AllFrames);
}

vector<int> Processor::createFreqMask(double fLow, double fHigh) const
{
    vector<int> indices;

    for(unsigned int i = 0; i < m_numberOfFrames; i++)
    {
        auto mask = (double)i/m_numberOfFrames*m_samplingRate;
        if (mask >= fLow && mask <= fHigh)
            indices.push_back(i);
    }
    return indices;
}


void Processor::amplify(double fLow, double fHight, double ampFactor)
{
    normalize(m_AllFrames,255);

    rgb2yiq(m_AllFrames, m_frameWidth, m_frameHeight, m_numberOfFrames, false);

    vector<int> mask = createFreqMask(fLow,fHight);

    complex_vector out_fft(m_numberOfFrames);
    vector<double> in_fft(m_numberOfFrames);

    fftw_plan p = fftw_plan_dft_r2c_1d((int)in_fft.size(), in_fft.data(), reinterpret_cast<fftw_complex*>(out_fft.data()), FFTW_ESTIMATE);

    complex_vector in_ifft(m_numberOfFrames);
    vector<double> out_ifft(m_numberOfFrames);

    fftw_plan p_ifft = fftw_plan_dft_c2r_1d(m_numberOfFrames, reinterpret_cast<fftw_complex*>(in_ifft.data()), out_ifft.data(), FFTW_ESTIMATE);

    for(unsigned int row = 0; row < m_frameHeight; row++)
        for(unsigned int col = 0; col < m_frameWidth; col++)
            for(unsigned int channel = 0; channel < channels; channel++)
            {
                vector<double> tmp = receive_pixel_values(row,col,channel);
                in_fft.assign(tmp.begin(),tmp.end());
                fftw_execute(p);

                complex_vector tmp1 = applyMask(out_fft,mask);
                in_ifft.assign(tmp1.begin(),tmp1.end());
                fftw_execute(p_ifft);

                normalize(out_ifft,(double)m_numberOfFrames/ampFactor);
                insert_pixel_values(out_ifft,row,col,channel);
            }
    fftw_destroy_plan(p);
    fftw_destroy_plan(p_ifft);
}


void Processor::AddPulseToFrames(vector<unique_ptr<Mat> > &frames) const
{
    const unsigned int NofFrames = frames.size();
    const unsigned int FrWidth {frames[0]->getCols()};
    const unsigned int FrHeight {frames[0]->getRows()};
    const unsigned int FrChannels {frames[0]->getChannels()};
    const unsigned int frame_size {FrHeight*FrWidth*FrChannels};
    vector<double> fullFrame(frame_size);
    vector<double> pulseFrame(frame_size);

    for(unsigned int frame_number = 0; frame_number < NofFrames; frame_number++)
    {
        FramesToVector(frames[frame_number], fullFrame);
        normalize(fullFrame,255.0);
        rgb2yiq(fullFrame, FrHeight, FrWidth, 1, false);
        NearInterpolation(m_AllFrames,pulseFrame,m_frameWidth,m_frameHeight,FrWidth,FrHeight,
                          frame_number);
        std::transform(fullFrame.begin(), fullFrame.end(), pulseFrame.begin(), fullFrame.begin(),
                       std::plus<double>());                                                    //summing the vectors
        YIQ2RGBnormalizeColorChannels(fullFrame, FrWidth, FrHeight, 1);
        std::transform(fullFrame.begin(),fullFrame.end(),fullFrame.begin(),
                       [](double a){if (a > 1) return 1.0; if (a < 0) return 0.0; return a;});
        normalize(fullFrame,1.0/255);
        VectorToFrames(fullFrame, frames[frame_number]);
    }
}

unsigned int Processor::getFrH(void) const
{
    return m_frameHeight;
}

unsigned int Processor::getFrW(void) const
{
    return m_frameWidth;
}

unsigned int Processor::getNFr(void) const
{
    return m_numberOfFrames;
}

vector<double> Processor::receive_pixel_values(unsigned int row, unsigned int col, unsigned int channel) const
{
    vector<double> res(m_numberOfFrames);

    unsigned int coor = calc_pixel_coor(0,row,col,channel);

    for(size_t k = 0; k < res.size(); k++)
    {
        res[k] = m_AllFrames[coor];
        coor += m_frameHeight*m_frameWidth*channels;
    }

    return res;
}


void Processor::insert_pixel_values(const vector<double>& values, unsigned int row, unsigned int col, unsigned int channel)
{
    if (values.size() != m_numberOfFrames)
        throw std::exception();
    for(size_t k = 0; k < values.size(); k++)
        m_AllFrames[calc_pixel_coor(k,row,col,channel)] = values[k];

}


unsigned int Processor::calc_pixel_coor(unsigned int k, unsigned int row, unsigned int col, unsigned int channel) const
{
    auto width = m_frameWidth;
    auto height = m_frameHeight;
    return channels*(width*height*k+width*row+col)+channel;
}
