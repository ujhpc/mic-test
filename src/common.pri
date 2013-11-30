TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

HEADERS += $$files(common/*.h)

# fix no symbol in current context on GCC 4.8
debug:QMAKE_CXXFLAGS += -gdwarf-2

greaterThan(QT_MAJOR_VERSION, 4) {
  CONFIG += object_parallel_to_source
} else:equals(PWD, $$OUT_PWD) {
  CONFIG += object_with_source
}

# drop binaries one level up
equals(PWD, $$OUT_PWD) {
  DESTDIR = ..
} else {
  DESTDIR = $$OUT_PWD/..
}

# don't use SDK but call compilers directly on Mac when not using Clang
macx:!macx-clang {
  CONFIG -= sdk
}

# turn on OpenMP for GCC & ICC
*-g++-*|*-g++|*-icc|*-icc-* {
  QMAKE_CXXFLAGS += -fopenmp
  LIBS           += -fopenmp
}

# enable realtime library for Linux
linux-*:LIBS     += -lrt

INCLUDEPATH += . \
               ../lib/cmdline

c++11 {
  # workaround for missing old qmake c++11 config
  !greaterThan(QT_MAJOR_VERSION, 4) {
    *-g++-*|*-g++|*-icc|*-icc-*:QMAKE_CXXFLAGS += -std=c++0x
    else:QMAKE_CXXFLAGS += -std=c++11
  } else:*-icc|*-icc-* {
    # c++11 is not enabled in ICC spec
    QMAKE_CXXFLAGS   += -std=c++11
    macx {
      QMAKE_CXXFLAGS += -stdlib=libc++
      QMAKE_LFLAGS   += -stdlib=libc++
    }
  }
}

unix:*-icc|*-icc-* {
  QMAKE_CXXFLAGS += -no-intel-extensions
  QMAKE_LFLAGS   += -no-intel-extensions -static-intel
}
