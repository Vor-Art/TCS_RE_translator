TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG += qt
# CONFIG += c++2a c++1z c++14
 QMAKE_CXXFLAGS += -Wall

SOURCES += \
        fsm.cpp \
        main.cpp \
        regexpr.cpp

DISTFILES += \
    input.txt \
    input0.txt \
    input1.txt \
    input2.txt

HEADERS += \
    fsm.h \
    myerror.h \
    regexpr.h
