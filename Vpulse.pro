TARGET = Vpulse
TEMPLATE = subdirs
SUBDIRS = lib exec

lib.file = lib/lib.pro
exec.file = exec/exec.pro
#tests.file = tests/tests.pro

CONFIG += ordered

HEADERS += conf.h \
    calculator.h \
    mat.h \
    processor.h \
    videoreader.h \
