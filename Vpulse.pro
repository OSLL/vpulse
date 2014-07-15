#-------------------------------------------------
#
# Project created by QtCreator 2012-11-15T02:04:50
#
#-------------------------------------------------

TARGET = Vpulse
TEMPLATE = app

QT += core

QT -= gui

SOURCES += main.cpp\
    videoreader.cpp \
    processor.cpp

HEADERS  += videoreader.h \
    Conf.h \
    processor.h

#LIBS += -LC:/opencv/build/x64/vc12/lib/ -lopencv_core248 -lopencv_highgui248 -lopencv_imgproc248 -lopencv_features2d248 -lopencv_nonfree248
LIBS += -LC:/opencv/build/x64/vc12/lib/ -lopencv_core248d -lopencv_highgui248d -lopencv_imgproc248d -lopencv_features2d248d -lopencv_nonfree248d

INCLUDEPATH += C:\opencv\build\include \
    $$PWD/fftw

#QMAKE_CXXFLAGS="-stdlib=libstdc++"

#CONFIG += c++11
LIBS += -L$$PWD/fftw/ -llibfftw3-3

DEPENDPATH += $$PWD/fftw
PRE_TARGETDEPS += $$PWD/fftw/libfftw3-3.lib



#PRE_TARGETDEPS += $$PWD/ffmpeg/lib/avcodec.lib

win32: LIBS += -L$$PWD/ffmpeg/lib/ -lavcodec-55 -lavformat-55 -lswscale-2 -lavutil-52

INCLUDEPATH += $$PWD/ffmpeg/include
DEPENDPATH += $$PWD/ffmpeg/include
