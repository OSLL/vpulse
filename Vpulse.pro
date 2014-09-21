TARGET = Vpulse
TEMPLATE = subdirs

CONFIG( release, debug|release ) {
SUBDIRS = lib exec test
}
CONFIG( debug, debug|release ) {
SUBDIRS = lib exec test unittest
}
lib.file = lib/lib.pro
exec.file = exec/exec.pro
test.file = test/test.pro
unittest.file = unittest/unittest.pro

CONFIG += ordered

HEADERS += conf.h \
    calculator.h \
    mat.h \
    processor.h \
    videoreader.h \
