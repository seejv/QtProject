QMAKE_CXXFLAGS += -std=c++0x
QT +=sql
QT       += core gui
CONFIG += c++11

INCLUDEPATH += $$PWD/ #头文件路径
DEPENDPATH += $$PWD/ #编译时依赖的相关路径

HEADERS += $$PWD/virtualkeyboard.h


SOURCES += $$PWD/virtualkeyboard.cpp

FORMS += $$PWD/virtualkeyboard.ui

RESOURCES += $$PWD/Resource/VirtualKeyBoardResource.qrc
