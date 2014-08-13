#ifndef MAT_H
#define MAT_H

#include<array>
#include "Conf.h"

using Vec3d = std::array<double, 3>;

const unsigned int s_channels = 3;

/*!
  Mat class is used to store a single image.
*/
class Mat
{
private:

    /*! Vector, storing the actual data */
    std::vector<double> m_data;
    /*! Rows number */
    unsigned int m_rows;
    /*! Cols number */
    unsigned int m_cols;
    /*! Channels number */

public:


    /*!
        Takes number of rows and cols. Allocates the memory needed.
    */
    Mat(unsigned int rows_in, unsigned int cols_in);

    //! Copy constructor
    Mat(const Mat& a);
    //! Move constructor
    Mat(Mat&& a);
    //! Copy assignment
    Mat& operator=(const Mat& a);
    //! at operator
    /*!
        Function for accessing a single value
        Takes pixel coordinates and the channel number
        \return Value by reference, so it can be edited
    */
    double &at(unsigned int row, unsigned int col, unsigned int channel);

    double at(unsigned int row, unsigned int col, unsigned int channel) const;
    /*!
        Function for accessing the full pixel value
        Takes pixel coordinates
        \return array of 3, containing pixel values across 3 channels
    */
    Vec3d getVec(unsigned int row, unsigned int col);

    /*!
        \return Number of rows
    */
    unsigned int getRows() const;
    /*!
        \return Number of cols
    */
    unsigned int getCols() const;
    /*!
        \return Number of channels
    */
    unsigned int getChannels() const;
    /*!
        \return Vector, containing image data by reference
    */
    std::vector<double>& data();

    /*!
        \return Size of the image
    */
    size_t size() const;

};

#endif // MAT_H
