#-------------------------------------------------
#
# Project created by QtCreator 2012-11-15T02:04:50
#
#-------------------------------------------------

QT       += core gui

TARGET = Vpulse
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    videoreader.cpp \
    processor.cpp

HEADERS  += mainwindow.h \
    videoreader.h \
    Conf.h \
    processor.h

FORMS    += mainwindow.ui


LIBS+=`pkg-config --cflags --libs libavformat libavcodec libavutil libswscale opencv`

#INCLUDEPATH+=$$PWD/../ffmpegL/ffmpeg/libavcodec
#INCLUDEPATH+=$$PWD/../ffmpegL/ffmpeg/libavformat
#INCLUDEPATH+=$$PWD/../ffmpegL/ffmpeg/libavutil
#INCLUDEPATH+=$$PWD/../ffmpegL/ffmpeg/libswscale



#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../ffmpegL/ffmpeg/libavformat/release/ -lavformat
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../ffmpegL/ffmpeg/libavformat/debug/ -lavformat
#else:symbian: LIBS += -lavformat
#else:unix: LIBS += -L$$PWD/../../ffmpegL/ffmpeg/libavformat/ -lavformat

INCLUDEPATH += $$PWD/../../ffmpegL/ffmpeg/libavformat
DEPENDPATH += $$PWD/../../ffmpegL/ffmpeg/libavformat

#win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../ffmpegL/ffmpeg/libavformat/release/avformat.lib
#else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../ffmpegL/ffmpeg/libavformat/debug/avformat.lib
#else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../../ffmpegL/ffmpeg/libavformat/libavformat.a


#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../ffmpegL/ffmpeg/libavcodec/release/ -lavcodec
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../ffmpegL/ffmpeg/libavcodec/debug/ -lavcodec
#else:symbian: LIBS += -lavcodec
#else:unix: LIBS += -L$$PWD/../../ffmpegL/ffmpeg/libavcodec/ -lavcodec

INCLUDEPATH += $$PWD/../../ffmpegL/ffmpeg/libavcodec
DEPENDPATH += $$PWD/../../ffmpegL/ffmpeg/libavcodec

#win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../ffmpegL/ffmpeg/libavcodec/release/avcodec.lib
#else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../ffmpegL/ffmpeg/libavcodec/debug/avcodec.lib
#else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../../ffmpegL/ffmpeg/libavcodec/libavcodec.a



#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../ffmpegL/ffmpeg/libavutil/release/ -lavutil
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../ffmpegL/ffmpeg/libavutil/debug/ -lavutil
#else:symbian: LIBS += -lavutil
#else:unix: LIBS += -L$$PWD/../../ffmpegL/ffmpeg/libavutil/ -lavutil

INCLUDEPATH += $$PWD/../../ffmpegL/ffmpeg/libavutil
DEPENDPATH += $$PWD/../../ffmpegL/ffmpeg/libavutil

#win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../ffmpegL/ffmpeg/libavutil/release/avutil.lib
#else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../ffmpegL/ffmpeg/libavutil/debug/avutil.lib
#else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../../ffmpegL/ffmpeg/libavutil/libavutil.a



#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../ffmpegL/ffmpeg/libswscale/release/ -lswscale
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../ffmpegL/ffmpeg/libswscale/debug/ -lswscale
#else:symbian: LIBS += -lswscale
#else:unix: LIBS += -L$$PWD/../../ffmpegL/ffmpeg/libswscale/ -lswscale

INCLUDEPATH += $$PWD/../../ffmpegL/ffmpeg/libswscale
DEPENDPATH += $$PWD/../../ffmpegL/ffmpeg/libswscale

#win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../ffmpegL/ffmpeg/libswscale/release/swscale.lib
#else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../ffmpegL/ffmpeg/libswscale/debug/swscale.lib
#else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../../ffmpegL/ffmpeg/libswscale/libswscale.a

INCLUDEPATH += $$PWD/../../ffmpegL/ffmpeg


#LIBS+=`pkg-config opencv --cflags --libs`



INCLUDEPATH+=\usr\local\lib\include\

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../usr/local/lib/release/ -lopencv_core
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../usr/local/lib/debug/ -lopencv_core
#else:symbian: LIBS += -lopencv_core
#else:unix: LIBS += -L$$PWD/../../../usr/local/lib/ -lopencv_core



INCLUDEPATH += $$PWD/../../../usr/local/include
DEPENDPATH += $$PWD/../../../usr/local/include
