TARGET = Vpulse

TEMPLATE = app

QT += testlib

DEFINES += QT_NODLL

#SOURCES += test.cpp

#HEADERS  += test.h

INCLUDEPATH += $$(FFTW_PATH)
INCLUDEPATH += $$(FFMPEG_PATH)/include
INCLUDEPATH += ..
INCLUDEPATH += $$(CXXTEST_PATH)

LIBS += -L$$(FFTW_PATH) -llibfftw3-3
LIBS += -L$$(FFMPEG_PATH)/lib/ -lavcodec-55 -lavformat-55 -lswscale-2 -lavutil-52
LIBS += -L../lib/release -lVpulse

