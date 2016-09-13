#-------------------------------------------------
#
# Project created by QtCreator 2016-08-22T20:25:25
#
#-------------------------------------------------

# using external library for fast fourier - kfr

QT       += core gui xml network

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
    player.cpp \
    alsa-debugger.cpp \
    Utils/ring-buffer.cpp \
    Concurent/pjusa-thread.cpp \
    Concurent/thread.cpp

HEADERS  += \
    gui.h \
    mem-pool.h \
    sipapp.h \
    defs.h \
    recorder.h \
    confbridge.h \
    mediaport.h \
    player.h \
    External/kfr/base/abs.hpp \
    External/kfr/base/asin_acos.hpp \
    External/kfr/base/atan.hpp \
    External/kfr/base/basic_expressions.hpp \
    External/kfr/base/clamp.hpp \
    External/kfr/base/compiletime.hpp \
    External/kfr/base/complex.hpp \
    External/kfr/base/constants.hpp \
    External/kfr/base/cpuid.hpp \
    External/kfr/base/cpuid_auto.hpp \
    External/kfr/base/digitreverse.hpp \
    External/kfr/base/expression.hpp \
    External/kfr/base/function.hpp \
    External/kfr/base/gamma.hpp \
    External/kfr/base/generators.hpp \
    External/kfr/base/hyperbolic.hpp \
    External/kfr/base/intrinsics.h \
    External/kfr/base/kfr.h \
    External/kfr/base/log_exp.hpp \
    External/kfr/base/logical.hpp \
    External/kfr/base/memory.hpp \
    External/kfr/base/min_max.hpp \
    External/kfr/base/modzerobessel.hpp \
    External/kfr/base/operators.hpp \
    External/kfr/base/platform.hpp \
    External/kfr/base/pointer.hpp \
    External/kfr/base/random.hpp \
    External/kfr/base/read_write.hpp \
    External/kfr/base/reduce.hpp \
    External/kfr/base/round.hpp \
    External/kfr/base/saturation.hpp \
    External/kfr/base/select.hpp \
    External/kfr/base/shuffle.hpp \
    External/kfr/base/simd.hpp \
    External/kfr/base/sin_cos.hpp \
    External/kfr/base/small_buffer.hpp \
    External/kfr/base/sort.hpp \
    External/kfr/base/specializations.i \
    External/kfr/base/sqrt.hpp \
    External/kfr/base/tan.hpp \
    External/kfr/base/types.hpp \
    External/kfr/base/univector.hpp \
    External/kfr/base/vec.hpp \
    External/kfr/cometa/array.hpp \
    External/kfr/cometa/cstring.hpp \
    External/kfr/cometa/ctti.hpp \
    External/kfr/cometa/function.hpp \
    External/kfr/cometa/named_arg.hpp \
    External/kfr/cometa/range.hpp \
    External/kfr/cometa/result.hpp \
    External/kfr/cometa/string.hpp \
    External/kfr/cometa/tuple.hpp \
    External/kfr/data/bitrev.hpp \
    External/kfr/data/sincos.hpp \
    External/kfr/dft/bitrev.hpp \
    External/kfr/dft/cache.hpp \
    External/kfr/dft/conv.hpp \
    External/kfr/dft/fft.hpp \
    External/kfr/dft/ft.hpp \
    External/kfr/dft/reference_dft.hpp \
    External/kfr/dsp/biquad.hpp \
    External/kfr/dsp/biquad_design.hpp \
    External/kfr/dsp/dcremove.hpp \
    External/kfr/dsp/delay.hpp \
    External/kfr/dsp/fir.hpp \
    External/kfr/dsp/fir_design.hpp \
    External/kfr/dsp/fracdelay.hpp \
    External/kfr/dsp/goertzel.hpp \
    External/kfr/dsp/impulse.hpp \
    External/kfr/dsp/interpolation.hpp \
    External/kfr/dsp/mixdown.hpp \
    External/kfr/dsp/oscillators.hpp \
    External/kfr/dsp/resample.hpp \
    External/kfr/dsp/sample_rate_conversion.hpp \
    External/kfr/dsp/speaker.hpp \
    External/kfr/dsp/units.hpp \
    External/kfr/dsp/waveshaper.hpp \
    External/kfr/dsp/weighting.hpp \
    External/kfr/dsp/window.hpp \
    External/kfr/io/audiofile.hpp \
    External/kfr/io/file.hpp \
    External/kfr/io/python_plot.hpp \
    External/kfr/io/tostring.hpp \
    External/kfr/all.hpp \
    External/kfr/base.hpp \
    External/kfr/cident.h \
    External/kfr/cometa.hpp \
    External/kfr/dft.hpp \
    External/kfr/dsp.hpp \
    External/kfr/io.hpp \
    External/kfr/math.hpp \
    External/kfr/version.hpp \
    alsa-debugger.h \
    Utils/ring-buffer.h \
    Concurent/pjsua-thread.h \
    Concurent/thread.h

FORMS    +=
