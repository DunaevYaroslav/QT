QMAKE_CXXFLAGS += -Werror
CONFIG += debug
HEADERS       = \
    calc.h \
    my_window.h
SOURCES       = main.cpp \
                calc.cpp \
                my_window.cpp
QT += widgets
