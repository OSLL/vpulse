#ifndef CONF_H
#define CONF_H

#include <QtGui/QApplication>
#include<QtGui/QPainter>
#include<QtGui/QLabel>
#include<QtGui/QPushButton>
#include<QtGui/QPixmap>
#include <QTime>
#include <QTextEdit>
#include "mainwindow.h"
#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif

#include <stdio.h>
#include <cstdio>
extern "C" {
#include <avcodec.h>
#include <avformat.h>
#include <swscale.h>
#include <version.h>        //swscale

#include "fftw3.h"
//#include "fft"
}

#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <QDir>


const int PRateInt =60;
typedef float transRgbYiqMatr[9];
const transRgbYiqMatr rgb2yiqCoef={0.2989, 0.587, 0.144, 0.5959, -0.2744, -0.3216, 0.2115, -0.5229, 0.3114};
const transRgbYiqMatr yiq2rgbCoef={1.000, 0.956, 0.621, 1.000, -0.272, -0.647, 1.000, -1.106, 1.703};
const transRgbYiqMatr ker={1.0/16.0, 2.0/16.0, 1.0/16.0, 2.0/16.0, 4.0/16.0, 2.0/16.0, 1.0/16.0, 2.0/16.0, 1.0/16.0};
#endif // CONF_H
