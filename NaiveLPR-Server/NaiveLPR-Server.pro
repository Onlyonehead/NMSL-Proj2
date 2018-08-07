#-------------------------------------------------
#
# Project created by QtCreator 2018-07-31T12:05:43
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NaiveLPR-Server
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

unix{
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib -lopencv_imgproc -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_ml

}

win32{
    INCLUDEPATH += D:\QTopencv\include\opencv\
               D:\QTopencv\include\opencv2\
               D:\QTopencv\include
    LIBS += D:\QTopencv\lib\libopencv_core342.dll.a\
            D:\QTopencv\lib\libopencv_highgui342.dll.a\
            D:\QTopencv\lib\libopencv_imgcodecs342.dll.a\
            D:\QTopencv\lib\libopencv_imgproc342.dll.a\
            D:\QTopencv\lib\libopencv_ml342.dll.a\

}

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp \
    forgetpassword.cpp \
    login.cpp \
    myserver.cpp \
    processor.cpp \
    socketthread.cpp \
    sqltool.cpp \
    TCPConnection.cpp \
    tool.cpp \
    CharRecognition.cpp \
    CharSegment.cpp \
    PlateDetection.cpp \
    PlateRecognition.cpp

HEADERS  += \
    forgetpassword.h \
    login.h \
    myserver.h \
    processor.h \
    socketthread.h \
    sqltool.h \
    TCPConnection.h \
    tool.h \
    CharRecognition.h \
    CharSegment.h \
    PlateDetection.h \
    PlateRecognition.h


win32{
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../packages/build-SMTPEmail-Desktop_Qt_5_7_1_MinGW_32bit-Debug/release/ -lSMTPEmail
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../packages/build-SMTPEmail-Desktop_Qt_5_7_1_MinGW_32bit-Debug/debug/ -lSMTPEmail
else:unix: LIBS += -L$$PWD/../packages/build-SMTPEmail-Desktop_Qt_5_7_1_MinGW_32bit-Debug/ -lSMTPEmail
INCLUDEPATH += $$PWD/../packages/build-SMTPEmail-Desktop_Qt_5_7_1_MinGW_32bit-Debug/debug
DEPENDPATH += $$PWD/../packages/build-SMTPEmail-Desktop_Qt_5_7_1_MinGW_32bit-Debug/debug
}

unix{
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../packages/build-SMTPEmail-Desktop_Qt_5_7_1_clang_64bit-Debug/release/ -lSMTPEmail.1.0.0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../packages/build-SMTPEmail-Desktop_Qt_5_7_1_clang_64bit-Debug/debug/ -lSMTPEmail.1.0.0
else:unix: LIBS += -L$$PWD/../packages/build-SMTPEmail-Desktop_Qt_5_7_1_clang_64bit-Debug/ -lSMTPEmail.1.0.0
INCLUDEPATH += $$PWD/../packages/build-SMTPEmail-Desktop_Qt_5_7_1_clang_64bit-Debug
DEPENDPATH += $$PWD/../packages/build-SMTPEmail-Desktop_Qt_5_7_1_clang_64bit-Debug
}
