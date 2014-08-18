TARGET = Vpulse
TEMPLATE = subdirs
SUBDIRS = lib exec tests

lib.file = lib/lib.pro
exec.file = exec/exec.pro
tests.file = tests/tests.pro

CONFIG += ordered
