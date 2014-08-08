#include "Mat.h"


Mat::Mat(int rows_in, int cols_in, int channels_in):
    rows(rows_in),
    cols(cols_in),
    channels(channels_in),
    data(rows_in*cols_in*channels_in)
{

}

Mat::Mat(const Mat& a):
    rows(a.rows),
    cols(a.cols),
    channels(a.channels),
    data(a.rows*a.cols*a.channels)
{
    this->data = a.data;
}

Mat::Mat(Mat&& a):
    rows(a.rows),
    cols(a.cols),
    channels(a.channels),
    data(a.rows*a.cols*a.channels)
{
    this->data = std::move(a.data);
}


Mat& Mat::operator=(const Mat& a)
{
    rows = a.rows;
    cols = a.cols;
    channels = a.channels;
    this->data = a.data;
    return *this;
}

double& Mat::at(int row, int col, int channel)
{
    //cout << (channels*(cols*row+col)+channel) << endl;
    return data[channels*(cols*row+col)+channel];
}


double* Mat::getVec(int row, int col)
{
    return &data[channels*(cols*row+col)];
}


int Mat::get_rows()
{
    return rows;
}


int Mat::get_cols()
{
    return cols;
}


int Mat::get_channels()
{
    return channels;
}

