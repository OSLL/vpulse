#ifndef CALCULATOR_H
#define CALCULATOR_H

#endif // CALCULATOR_H

#include <utility>
#include <conf.h>
#include <videoreader.h>
#include <processor.h>
#include <mat.h>

using harmonic_stat = pair<double,double>;   //a data type for holding information about an harmonic oscillation, first is amplitude, second is period
using Point = pair<size_t,size_t>;           //a data type for holding pixel coordinates on the image.

namespace Calculator
{

/*!
    Function calculates amplitude and period of the oscillation represented by the array of values
    \param values values vector
    \return harmonic stats of the oscillation
*/
harmonic_stat calc_amplitude_and_period(vector<double> values);

/*!
    From the array of periods, function find k with least difference and returns their average
    \param periods vector of periods
    \param k parameter
    \return Average of k periods with least difference
*/
double calc_average_significant_period(vector<double> periods, size_t k);

/*!
    Generates array of values of sin function to be oscillation with specified parameters
    \param length length of output array
    \param ampl amplitude of the oscillation
    \param period period of the oscillation
    \return vector of values of the oscillation
*/
vector<double> gen_sin_vector(size_t length, double ampl, double period);


/*!
    Function generates a set of images so that its pixels are from oscillation with specified parameters
    \param length length of vector of images
    \param width width of images
    \param height height of images
    \param ampl amplitude of oscillation
    \param period period of oscillation
    \return a vector of unique pointers to the Mat images
*/
vector<unique_ptr<Mat>> gen_test_image(size_t length, size_t width, size_t height, double ampl, double period);

/*!
    Function obtains an array of pixel values from the images vector
    \param src a vector of unique pointers to the Mat images
    \param row row
    \param col column
    \param channel channel
    \return vector of specified pixel values
*/
vector<double> receive_pixel_values(vector<unique_ptr<Mat>>& src, size_t row, size_t col, size_t channel);

/*!
    Function obtains an array of pixel values from the images vector in specified range [start;end)
    \param src a vector of unique pointers to the Mat images
    \param row row
    \param col column
    \param channel channel
    \param start Starting position
    \param end Ending position
    \return vector of values in specified range [start;end)

*/
vector<double> receive_pixel_values(vector<unique_ptr<Mat>>& src, size_t row, size_t col, size_t channel, size_t start, size_t end);

/*!
    Function obtains an array of averaged pixel values from the images vector in specified range [start;end)
    \param src a vector of unique pointers to the Mat images
    \param row row
    \param col column
    \param area_size radius of circle which area will be used to average pixel value
    \param channel channel
    \param start Starting position
    \param end Ending position
    \return vector of averaged pixel values in specified range [start; end)
*/
vector<double> receive_averaged_pixel_values(vector<unique_ptr<Mat>>& src, size_t row, size_t col, double area_size, size_t start, size_t end);


/*!
    Function calculates pulse from video according to points, specified by the parameter
    \param fr1 lower frequency threshold
    \param fr2 upper frequency threshold
    \param ampFactor amplifying factor
    \param avg_parameter
    \param area_radius radius of circle used to average pixel values. 1 for no averaging
    \return A vector of real numbers, one for each period of time with length equal to upd_freq parameter
*/
vector<double> calculate_pulse(VideoReader& video, vector<Point>& points, double upd_freq, double fr1, double fr2, double ampFactor, size_t avg_parameter, double area_radius);

/*!
    Function generates standart points for image with specified width and height.
    \param width width
    \param height height
    \return a vector of Points
*/
vector<Point> standart_points(size_t width, size_t height);

/*!
    Function finds points of interest in the video for potential pulse recognition
    \param video VideoReader video
    \return a vector of Points
*/
vector<Point> find_points_of_interest(VideoReader& video);

}
