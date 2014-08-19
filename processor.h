#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <conf.h>
#include <mat.h>

using transRgbYiqMatr = double[9];
using std::vector;
using std::unique_ptr;

const transRgbYiqMatr rgb2yiqCoef {0.2989, 0.587, 0.144, 0.5959, -0.2744, -0.3216, 0.2115, -0.5229, 0.3114};
const transRgbYiqMatr yiq2rgbCoef {1.000, 0.956, 0.621, 1.000, -0.272, -0.647, 1.000, -1.106, 1.703};

const int channels = 3;

/*!
  Processor class is used to store and process a blurred image to create pulse frames.
*/
class Processor
{
private:

    /*! Number of frames */
    unsigned int m_numberOfFrames;
    /*! Frame height */
    unsigned int m_frameHeight;
    /*! Frame width */
    unsigned int m_frameWidth;
    /*! Sampling rate of the video */
    unsigned int m_samplingRate;
    /*! Vector, storing the actual data */
    vector<double> m_AllFrames;


    /*!
        Function for calculating pixel coordinate in m_AllFrames
        Takes the image number, pixel coordinates and the channel number
        \return value position in the vector
    */
    unsigned int calc_pixel_coor(unsigned int k, unsigned int row, unsigned int col, unsigned int channel) const;

public:

    /*!
        Function for creating the mask of elements with frequencies between fLow and fHigh
        Mask can be used against DFT array of the same length as number of images in current object
        Takes the lower and higher bounds of frequencies
        \return vector, containing indexes of elements with allowed frequencies
    */
    vector<int> createFreqMask(double fLow, double fHigh) const;

    /*!
        \param sRate_in The sampling rate of the video used to record the images
        \param Frames Vector of pointers to Mat, containing images
    */
    Processor(unsigned int sRate_in, const vector<unique_ptr<Mat> > &Frames);

    /*!
        Filters the frequencies to those between the bounds and amplifies them
        \param fLow lower frequency bound
        \param fHigh higher frequency bound
        \param ampFactor amplifying factor
    */
    void amplify(double fLow, double fHight, double ampFactor);

    /*!
        Function for receiving an array of values for the chosen pixel over time
        Takes coordinates of the pixel and the channel number
        \return vector, containing the values of the pixel channel-th value over time
    */
    vector<double> receive_pixel_values(unsigned int row, unsigned int col, unsigned int channel) const;

    /*!
        Insert pixel values over time into the Processor data
        Takes pixel coordinates, channel number and array of values
        \param values vector, containing values
    */
    void insert_pixel_values(const vector<double>& values, unsigned int row, unsigned int col, unsigned int channel);

    /*!
        \return Frame height
    */
    unsigned int getFrH() const;

    /*!
        \return Frame width
    */
    unsigned int getFrW() const;

    /*!
        \return Number of frames
    */
    unsigned int getNFr() const;

    /*!
        Function for adding the pulse frames onto input frames
    */
    void AddPulseToFrames(vector<unique_ptr<Mat>>& frames) const;

};

namespace ProcessorUtilities
{

/*!
    Function converts YIQ image to RGB and normalizes values to be between 0 and 1
*/
void YIQ2RGBnormalizeColorChannels(vector<double>& srcDst, int frWidth, int frHeight, int NofFrames);

/*!
    Function interpolates an image of old size to the image with new size by finding the nearest propotional coordinate match
*/
void NearInterpolation(const vector<double>& src, vector<double>& dst, int oldwidth, int oldheight, int newwidth, int newheight, int frameInd);
/*!
    Function extracts values vector from vector of frames
*/
void FramesToVector(const vector<unique_ptr<Mat> > &src, vector<double>& dst);
/*!
    Function extracts values vector from a frame
*/
void FramesToVector(const unique_ptr<Mat>& src, vector<double>& dst);
/*!
    Function converts values vector to a frame
*/
void VectorToFrames(const vector<double>& src, unique_ptr<Mat>& dst);
/*!
    Function applies mask, generated in createFreqMask to src vector
    \return result vector
*/
complex_vector applyMask(const complex_vector& src, const vector<int>& mask);
}

#endif // PROCESSOR_H
