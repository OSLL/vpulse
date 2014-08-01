#ifndef CONF_H
#define CONF_H

#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif

#include <cstdio>

extern "C"
{

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswscale/version.h>
}

#include <complex>

extern "C"
{
#include <fftw3.h>
}
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <QDir>

using complex_number = std::complex<double>;
using complex_vector = std::vector<std::complex<double>>;
#endif // CONF_H
