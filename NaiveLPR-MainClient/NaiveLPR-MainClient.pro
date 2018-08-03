#-------------------------------------------------
#
# Project created by QtCreator 2018-07-30T14:45:06
#
#-------------------------------------------------

QT       += core gui network sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NaiveLPR-MainClient
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH +=/usr/local/include
LIBS +=-L/usr/local/lib -lopencv_imgproc -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_ml

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainclient.cpp \
    forgetpassword.cpp \
    login.cpp \
    mainclient_msg.cpp \
    page_lpr1.cpp \
    page_lpr2.cpp \
    CharRecognition.cpp \
    settings.cpp

HEADERS  += mainclient.h \
    forgetpassword.h \
    login.h \
    CharRecognition.h

FORMS    += mainclient.ui \
    forgetpassword.ui \
    login.ui

RESOURCES += \
    ../Resources/resources.qrc
