#ifndef CALCULATOR_H
#define CALCULATOR_H

#endif // CALCULATOR_H

#include <utility>
#include <conf.h>
#include <videoreader.h>
#include <processor.h>
#include <mat.h>

using harmonic_stat = pair<double,double>;
using Point = pair<size_t,size_t>;

namespace Calculator
{

/*!
    Function calculates amplitude and period of the oscillation represented by the array of values
    \return a pair of values. First is amplitude, second is period
*/
harmonic_stat calc_amplitude_and_period(vector<double> values);

/*!
    From the array of periods, find k with least difference and returns their average
*/
double calc_average_significant_period(vector<double> periods, size_t k);

/*!
    Generates array of values of sin function to be oscillation with specified parameters
    \param length length of output array
    \param ampl amplitude of oscillation
    \param period period of oscillation
*/
std::vector<double> gen_sin_vector(size_t length, double ampl, double period);


/*!
    Function generates a set of images so that its pixels are from oscillation with specified parameters
    \param length length of vector of images
    \param width width of images
    \param height height of images
    \param ampl amplitude of oscillation
    \param period period of oscillation
*/
std::vector<std::unique_ptr<Mat>> gen_test_image(size_t length, size_t width, size_t height, double ampl, double period);

/*!
    Function obtains an array of pixel values from the images vector
*/
std::vector<double> receive_pixel_values(vector<unique_ptr<Mat>>& src, size_t row, size_t col, size_t channel);

/*!
    Function obtains an array of pixel values from the images vector
*/
std::vector<double> receive_averaged_pixel_values(std::vector<std::unique_ptr<Mat>>& src, size_t row, size_t col, double area_size);

/*!
    Function calculates pulse from video according to points, specified by the parameter
    \param fr1
    \param fr2
    \param ampFactors
    \param avg_parameter
    \param area_radius
*/
double calculate_pulse(VideoReader& video, vector<Point>& points, double fr1, double fr2, double ampFactor, size_t avg_parameter, double area_radius);

/*!
    Function generates standart points for image with specified width and height.
*/
std::vector<Point> standart_points(size_t width, size_t height);

/*!
    Function finds points of interest in the video for potential pulse recognition
*/
vector<Point> find_points_of_interest(VideoReader& video);

}
