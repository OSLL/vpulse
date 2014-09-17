#include <processor.h>
#include <functional>
#include <cmath>

using namespace std;

namespace ProcessorUtilities
{

inline double UcharToDouble(unsigned char a)
{
    return static_cast<double>(a);
}

inline unsigned char DoubleToUchar(double a)
{
    return static_cast<unsigned char>(a);
}


void MatVectorToVector(const vector<unique_ptr<Mat>>& src, vector<double>& dst)
{
    const size_t image_size = src[0]->size();
    for(size_t i = 0; i < src.size(); i++)
        transform(src[i]->data().begin(), src[i]->data().end(), dst.begin()+ i*image_size, UcharToDouble);
}

void MatToVector(Mat& src, vector<double>& dst)
{
    transform(src.data().begin(), src.data().end(), dst.begin(), UcharToDouble);
}


void VectorToMat(const vector<double>& src, Mat& dst)
{
    transform(src.begin(), src.end(), dst.data().begin(), DoubleToUchar);
}

void normalize(vector<double>& src, double factor)
{
    for(auto& e : src)
        e/=factor;
}


complex_vector applyMask(const complex_vector& src, const vector<size_t>& mask)
{
    complex_vector dst(src.size(), complex_number(0,0));

    for(auto i : mask) //copy src data in elements with indices from mask
    {
        dst[i] = src[i];
    }
    return dst;
}

void rgb2yiq(vector<double>& srcDst, size_t frWidth, size_t frHeight, size_t NofFrames, bool rev)
{
    size_t pixels = NofFrames*frHeight*frWidth*channels;
    const double* Coefs = rev ? &yiq2rgbCoef[0] : &rgb2yiqCoef[0];
    for(size_t i = 0; i < pixels; i+=3)
    {
       double tmp[3] {srcDst[i], srcDst[i + 1], srcDst[i + 2]};  //ugly code, could be done in one line using linear algebra
       srcDst[i + 0] = tmp[0]*Coefs[0] + tmp[1]*Coefs[1] + tmp[2]*Coefs[2];
       srcDst[i + 1] = tmp[0]*Coefs[3] + tmp[1]*Coefs[4] + tmp[2]*Coefs[5];
       srcDst[i + 2] = tmp[0]*Coefs[6] + tmp[1]*Coefs[7] + tmp[2]*Coefs[8];
    }
}

void YIQ2RGBnormalizeColorChannels(vector<double>& srcDst, size_t frWidth, size_t frHeight, size_t NofFrames)
{
    rgb2yiq(srcDst,frWidth,frHeight,NofFrames,true);

    size_t pixels = NofFrames*frHeight*frWidth*channels;
    for(size_t i = 0; i < pixels; i+=3)
    {
        double normfactor {max({srcDst[i + 0], srcDst[i + 1], srcDst[i + 2], 1.0})};

       if (normfactor > 1.0)
       {
            for(size_t j = 0; j < channels; j++)
                srcDst[i + j]/=normfactor;
       }
    }
}

void NearInterpolation(const vector<double>& src, vector<double>& dst, size_t oldwidth, size_t oldheight, size_t newwidth, size_t newheight, size_t frameInd)
{
    const double scaleRateW = (double)newwidth / oldwidth;
    const double scaleRateH = (double)newheight / oldheight;
    for(int cx = 0; cx < newwidth; cx++)
        for(int cy = 0; cy < newheight; cy++)
        {
            size_t oldcx = static_cast<size_t>(floor((double)cx / scaleRateW));
            size_t oldcy = static_cast<size_t>(floor((double)cy / scaleRateH));
            size_t nearest_match {channels*(oldwidth*oldheight*frameInd+oldwidth*oldcy+oldcx)};
            size_t pixel_index {channels * (cy * newwidth + cx)};
            for(size_t i = 0; i < channels; i++)
                dst[pixel_index + i] = src[nearest_match + i];
        }
}

}

using namespace ProcessorUtilities;

Processor::Processor(size_t sRate_in, const vector<unique_ptr<Mat> > &Frames):
    m_numberOfFrames(Frames.size()),
    m_frameHeight(Frames[0]->getRows()),
    m_frameWidth(Frames[0]->getCols()),
    m_samplingRate(sRate_in),
    m_AllFrames(m_numberOfFrames*m_frameHeight*m_frameWidth*channels)
{
    MatVectorToVector(Frames, m_AllFrames);
}

vector<size_t> Processor::createFreqMask(double fLow, double fHigh) const
{
    vector<size_t> indices;

    for(size_t i = 0; i < m_numberOfFrames; i++)
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

    auto mask = createFreqMask(fLow,fHight);

    complex_vector out_fft(m_numberOfFrames);
    vector<double> in_fft(m_numberOfFrames);

    fftw_plan p = fftw_plan_dft_r2c_1d((int)in_fft.size(), in_fft.data(), reinterpret_cast<fftw_complex*>(out_fft.data()), FFTW_ESTIMATE);

    complex_vector in_ifft(m_numberOfFrames);
    vector<double> out_ifft(m_numberOfFrames);

    fftw_plan p_ifft = fftw_plan_dft_c2r_1d(static_cast<int>(m_numberOfFrames), reinterpret_cast<fftw_complex*>(in_ifft.data()), out_ifft.data(), FFTW_ESTIMATE);

    for(size_t row = 0; row < m_frameHeight; row++)
        for(size_t col = 0; col < m_frameWidth; col++)
            for(size_t channel = 0; channel < channels; channel++)
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
    const size_t NofFrames {frames.size()};
    const size_t FrWidth {frames[0]->getCols()};
    const size_t FrHeight {frames[0]->getRows()};
    const size_t FrChannels {frames[0]->getChannels()};
    const size_t frame_size {FrHeight*FrWidth*FrChannels};
    vector<double> fullFrame(frame_size);
    vector<double> pulseFrame(frame_size);

    for(size_t frame_number = 0; frame_number < NofFrames; frame_number++)
    {
        MatToVector(*frames[frame_number], fullFrame);
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
        VectorToMat(fullFrame, *frames[frame_number]);
    }
}

size_t Processor::getFrH(void) const
{
    return m_frameHeight;
}

size_t Processor::getFrW(void) const
{
    return m_frameWidth;
}

size_t Processor::getNFr(void) const
{
    return m_numberOfFrames;
}

vector<double> Processor::receive_pixel_values(size_t row, size_t col, size_t channel) const
{
    vector<double> res(m_numberOfFrames);

    size_t coor = calc_pixel_coor(0,row,col,channel);

    for(size_t k = 0; k < res.size(); k++)
    {
        res[k] = m_AllFrames[coor];
        coor += m_frameHeight*m_frameWidth*channels;
    }

    return res;
}


void Processor::insert_pixel_values(const vector<double>& values, size_t row, size_t col, size_t channel)
{
    if (values.size() != m_numberOfFrames)
        throw std::exception();
    for(size_t k = 0; k < values.size(); k++)
        m_AllFrames[calc_pixel_coor(k,row,col,channel)] = values[k];

}


inline size_t Processor::calc_pixel_coor(size_t k, size_t row, size_t col, size_t channel) const
{
    auto width = m_frameWidth;
    auto height = m_frameHeight;
    return channels*(width*height*k+width*row+col)+channel;
}
