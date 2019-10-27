#-------------------------------------------------
#
# Project created by QtCreator 2016-05-17T10:15:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EncDecryptor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        ARC4/arc4dialog.cpp \
        about.cpp \
        matchfiles.cpp \
        matchfilesobject.cpp \
        ARC4/arc4object.cpp \
        Rabbit/rabbitobject.cpp \
        Rabbit/rabbitdialog.cpp \
    Salsa20/salsa20dialog.cpp \
    Salsa20/salsa20object.cpp

HEADERS  += mainwindow.h \
        ARC4/arc4dialog.h \
        about.h \
        matchfiles.h \
        matchfilesobject.h \
        ARC4/arc4object.h \
        Rabbit/rabbitobject.h \
        Rabbit/rabbitdialog.h \
    Salsa20/salsa20dialog.h \
    Salsa20/salsa20object.h

FORMS    += mainwindow.ui \
        ARC4/arc4dialog.ui \
        about.ui \
        matchfiles.ui \
        Rabbit/rabbitdialog.ui \
    Salsa20/salsa20dialog.ui

QMAKE_CXXFLAGS = -std=c++11

RESOURCES += \
    enc_decryptor.qrc

RC_FILE = enc_decryptor.rc

DISTFILES += \
    enc_decryptor.rc \
    LICENSE.txt
