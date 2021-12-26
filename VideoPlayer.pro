QT += core gui opengl
QT += openglwidgets

CONFIG += c++14

INCLUDEPATH += -I /usr/local/Cellar/ffmpeg/4.4_2/include

LIBS += -L/usr/local/Cellar/ffmpeg/4.4_2/lib -lavutil -lavcodec -lavformat

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    VideoPlayer.cpp \
    VideoWidget.cpp

HEADERS += \
    VideoPlayer.h \
    VideoWidget.h

FORMS += \
    VideoPlayer.ui

