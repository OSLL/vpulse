TARGET = Vpulse

TEMPLATE = lib
CONFIG += staticlib

DEFINES += QT_NODLL

SOURCES +=\
    mat.cpp \
    processor.cpp \
    videoreader.cpp \
    calculator.cpp \

INCLUDEPATH += $$(FFTW_PATH)
INCLUDEPATH += $$(FFMPEG_INCLUDE_PATH)
INCLUDEPATH += ..

LIBS += -L$$(FFTW_PATH) -llibfftw3-3
LIBS += -L$$(FFMPEG_LIB_PATH) -lavcodec -lavformat -lswscale -lavutil
