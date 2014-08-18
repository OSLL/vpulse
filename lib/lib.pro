TARGET = Vpulse

TEMPLATE = lib
CONFIG += staticlib

DEFINES += QT_NODLL

SOURCES +=\
    Mat.cpp \
    Processor.cpp \
    Videoreader.cpp \
    Calculator.cpp \

INCLUDEPATH += $$(FFTW_PATH)
INCLUDEPATH += $$(FFMPEG_PATH)/include
INCLUDEPATH += ..

LIBS += -L$$(FFTW_PATH) -llibfftw3-3
LIBS += -L$$(FFMPEG_PATH)/lib/ -lavcodec-55 -lavformat-55 -lswscale-2 -lavutil-52
