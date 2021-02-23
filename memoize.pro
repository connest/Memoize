TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += optimize_full

QMAKE_CXXFLAGS += -pedantic-errors -Wall -Wextra -Weffc++

SOURCES += \
        main.cpp

HEADERS += \
    memoize.h
