#ifndef CONF_H
#define CONF_H

#include <QtGui/QApplication>
#include<QtGui/QPainter>
#include<QtGui/QLabel>
#include<QtGui/QPushButton>
#include<QtGui/QPixmap>
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

const int FRAMES_MAX =1000;
typedef float transRgbYiqMatr[9];
const transRgbYiqMatr rgb2yiqCoef={0.299, 0.587, 0.144, 0.596, -0.274, -0.322, 0.211, -0.522, 0.311};

#endif // CONF_H
