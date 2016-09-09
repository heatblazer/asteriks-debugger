#-------------------------------------------------
#
# Project created by QtCreator 2016-08-22T20:25:25
#
#-------------------------------------------------

QT       += core gui xml network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = asteriks-debugger
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11 -Wextra -pedantic -Wmissing-braces \
-Wparentheses -Wsequence-point -Wswitch -Wuninitialized -Wcast-qual \
-Wlogical-op -Wnormalized=nfkc


LIBS += -lpjsua2-x86_64-unknown-linux-gnu \
    -lpjsua-x86_64-unknown-linux-gnu \
    -lpjsip-ua-x86_64-unknown-linux-gnu \
    -lpjsip-simple-x86_64-unknown-linux-gnu \
    -lpjsip-x86_64-unknown-linux-gnu \
    -lpjmedia-codec-x86_64-unknown-linux-gnu \
    -lpjmedia-x86_64-unknown-linux-gnu \
    -lpjmedia-videodev-x86_64-unknown-linux-gnu \
    -lpjmedia-audiodev-x86_64-unknown-linux-gnu \
    -lpjmedia-x86_64-unknown-linux-gnu \
    -lpjnath-x86_64-unknown-linux-gnu \
    -lpjlib-util-x86_64-unknown-linux-gnu \
    -lsrtp-x86_64-unknown-linux-gnu \
    -lresample-x86_64-unknown-linux-gnu \
    -lpj-x86_64-unknown-linux-gnu \
    -lssl \
    -lcrypto \
    -luuid \
    -lasound

SOURCES += main.cpp \
    gui.cpp \
    mem-pool.cpp \
    sipapp.cpp \
    recorder.cpp \
    confbridge.cpp \
    mediaport.cpp \
    player.cpp

HEADERS  += \
    gui.h \
    mem-pool.h \
    sipapp.h \
    defs.h \
    recorder.h \
    confbridge.h \
    mediaport.h \
    player.h

FORMS    +=
