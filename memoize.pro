TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -pedantic-errors -Wall -Wextra

SOURCES += \
        main.cpp

HEADERS += \
    memoize.h
