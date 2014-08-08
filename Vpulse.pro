#-------------------------------------------------
#
# Project created by QtCreator 2012-11-15T02:04:50
#
#-------------------------------------------------

TARGET = Vpulse
TEMPLATE = app

QT += core

QT -= gui

DEFINES += QT_NODLL

SOURCES += main.cpp\
    videoreader.cpp \
    processor.cpp \
    Mat.cpp

HEADERS  += videoreader.h \
    Conf.h \
    processor.h \
    Mat.h

INCLUDEPATH += $$PWD/fftw

LIBS += -L$$PWD/fftw/ -llibfftw3-3

DEPENDPATH += $$PWD/fftw
PRE_TARGETDEPS += $$PWD/fftw/libfftw3-3.lib

LIBS += -L$$PWD/ffmpeg/lib/ -lavcodec-55 -lavformat-55 -lswscale-2 -lavutil-52
INCLUDEPATH += $$PWD/ffmpeg/include
DEPENDPATH += $$PWD/ffmpeg/include

