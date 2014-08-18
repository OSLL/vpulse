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

SOURCES +=\
    Mat.cpp \
    Processor.cpp \
    Videoreader.cpp \
    Calculator.cpp \
    main.cpp

HEADERS  += \
    Conf.h \
    Mat.h \
    Videoreader.h \
    Processor.h \
    Calculator.h

message($$(FFEMPG_PATH))
message($$(FFTW_PATH))

INCLUDEPATH += $$(FFTW_PATH)
INCLUDEPATH += $$(FFMPEG_PATH)/include

LIBS += -L$$(FFTW_PATH) -llibfftw3-3
LIBS += -L$$(FFMPEG_PATH)/lib/ -lavcodec-55 -lavformat-55 -lswscale-2 -lavutil-52
