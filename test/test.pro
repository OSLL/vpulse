TARGET = vpulsetest

TEMPLATE = app

DEFINES += QT_NODLL

CONFIG += c++11

SOURCES += main.cpp \
    processortest.cpp

INCLUDEPATH += $$(FFTW_PATH)
INCLUDEPATH += $$(FFMPEG_INCLUDE_PATH)
INCLUDEPATH += $$(GOOGLETEST_INCLUDE_PATH)
INCLUDEPATH += ..


LIBS += -L$$(FFTW_PATH) -llibfftw3-3
LIBS += -L$$(FFMPEG_LIB_PATH) -lavcodec -lavformat -lswscale -lavutil
LIBS += -L$$(GOOGLETEST_LIB_PATH) -lgtestd
release{
LIBS += -L../lib/release -lVpulse
}
debug{
LIBS += -L../lib/debug -lVpulse
}

