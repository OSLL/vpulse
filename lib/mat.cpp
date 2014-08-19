#include <mat.h>

Mat::Mat(size_t rows_in, size_t cols_in):
    m_rows(rows_in),
    m_cols(cols_in),
    m_data(rows_in*cols_in*s_channels)
{

}

Mat::Mat(const Mat& a):
    m_rows(a.m_rows),
    m_cols(a.m_cols),
    m_data(a.m_rows*a.m_cols*s_channels)
{
    this->m_data = a.m_data;
}

Mat::Mat(Mat&& a):
    m_rows(a.m_rows),
    m_cols(a.m_cols),
    m_data(a.m_rows*a.m_cols*s_channels)
{
    m_data = std::move(a.m_data);
}


Mat& Mat::operator=(const Mat& a)
{
    m_rows = a.m_rows;
    m_cols = a.m_cols;
    m_data = a.m_data;
    return *this;
}

double& Mat::at(size_t row, size_t col, size_t channel)
{
    return m_data[s_channels*(m_cols*row+col)+channel];
}

double Mat::at(size_t row, size_t col, size_t channel) const
{
    return m_data[s_channels*(m_cols*row+col)+channel];
}


Vec3d Mat::getVec(size_t row, size_t col)
{
    Vec3d res;
    for(size_t i = 0; i < s_channels; i++)
        res[i] = m_data[s_channels*(m_cols*row+col) + i];
    return res;
}


size_t Mat::getRows() const
{
    return m_rows;
}


size_t Mat::getCols() const
{
    return m_cols;
}

size_t Mat::getChannels() const
{
    return s_channels;
}

std::vector<double>& Mat::data()
{
    return m_data;
}

size_t Mat::size() const
{
    return m_data.size();
}

