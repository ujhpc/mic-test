TEMPLATE = subdirs

CONFIG += c++11

# in Qt5 on Mac we require calling cache of we get complaints
macx {
  greaterThan(QT_MAJOR_VERSION, 4): cache()
}

# don't use SDK but call compilers directly on Mac when not using Clang
macx:!macx-clang {
  CONFIG -= sdk
}

SUBDIRS += $$files(src/*.pro)

OTHER_FILES += $$files(scripts/*)
