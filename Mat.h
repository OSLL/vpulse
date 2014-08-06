#ifndef MAT_H
#define MAT_H

#include<vector>
#include "Conf.h"

using namespace std;

class Mat
{
private:

    vector<double> data;
    int rows;
    int cols;
    int channels;

public:

    Mat(int rows_in, int cols_in, int channels_in);
    Mat(const Mat& a);
    Mat(Mat&& a);
    Mat& operator=(const Mat& a);
    double &at(int row, int col, int channel);
    double* getVec(int row, int col);
    int get_rows();
    int get_cols();
    int get_channels();

};

#endif // MAT_H
