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
}

#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

const int FRAMES_MAX =1000;


#endif // CONF_H
