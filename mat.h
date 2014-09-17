#ifndef MAT_H
#define MAT_H

#include <array>
#include <conf.h>

const size_t s_channels = 3;

using Vec3d = std::array<double, s_channels>;
using Vec3b = std::array<unsigned char, s_channels>;

/*!
  Mat class is used to store a single image.
*/
class Mat
{
private:
    /*! Rows number */
    size_t m_rows;
    /*! Cols number */
    size_t m_cols;
    /*! Channels number */
    /*! Vector, storing the actual data */
    std::vector<unsigned char> m_data;

public:


    /*!
        Takes number of rows and cols. Allocates the memory needed.
    */
    Mat(size_t rows_in, size_t cols_in);

    //! at operator
    /*!
        Function for accessing a single value
        Takes pixel coordinates and the channel number
        \return Value by reference, so it can be edited
    */
    unsigned char &at(size_t row, size_t col, size_t channel);

    unsigned char at(size_t row, size_t col, size_t channel) const;
    /*!
        Function for accessing the full pixel value
        Takes pixel coordinates
        \return array of 3, containing pixel values across 3 channels
    */
    Vec3b getVec(size_t row, size_t col);

    /*!
        \return Number of rows
    */
    size_t getRows() const;
    /*!
        \return Number of cols
    */
    size_t getCols() const;
    /*!
        \return Number of channels
    */
    size_t getChannels() const;
    /*!
        \return Vector, containing image data by reference
    */
    std::vector<unsigned char> &data();

    /*!
        \return Size of the image
    */
    size_t size() const;

};

#endif // MAT_H
