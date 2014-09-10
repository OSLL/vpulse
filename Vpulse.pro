TARGET = Vpulse
TEMPLATE = subdirs
release{
SUBDIRS = lib exec
}
debug{
SUBDIRS = lib exec test
}
lib.file = lib/lib.pro
exec.file = exec/exec.pro

debug{
test.file = test/test.pro
}

CONFIG += ordered

HEADERS += conf.h \
    calculator.h \
    mat.h \
    processor.h \
    videoreader.h \
