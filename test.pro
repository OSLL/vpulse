#-------------------------------------------------
#
# Project created by QtCreator 2012-10-20T21:11:42
#
#-------------------------------------------------

QT       += core gui

TARGET = test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h


LIBS+=`pkg-config --cflags --libs libavformat libavcodec libavutil libswscale`

INCLUDEPATH+=$$PWD/../ffmpegLib/ffmpeg1/ffmpeg2/libavcodec
INCLUDEPATH+=$$PWD/../ffmpegLib/ffmpeg1/ffmpeg2/libavformat
INCLUDEPATH+=$$PWD/../ffmpegLib/ffmpeg1/ffmpeg2/libavutil
INCLUDEPATH+=$$PWD/../ffmpegLib/ffmpeg1/ffmpeg2/libswscale


