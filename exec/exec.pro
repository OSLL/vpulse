TARGET = Vpulse

TEMPLATE = app

DEFINES += QT_NODLL

CONFIG += c++11

SOURCES += main.cpp

INCLUDEPATH += $$(FFTW_PATH)
INCLUDEPATH += $$(FFMPEG_INCLUDE_PATH)
INCLUDEPATH += ..

LIBS += -L$$(FFTW_PATH) -llibfftw3-3
LIBS += -L$$(FFMPEG_LIB_PATH) -lavcodec -lavformat -lswscale -lavutil
release{
LIBS += -L../lib/release -lVpulse
}
debug{
LIBS += -L../lib/debug -lVpulse
}

