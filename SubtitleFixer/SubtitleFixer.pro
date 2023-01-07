#-------------------------------------------------
#
# Project created by QtCreator 2019-06-15T15:49:37
#
#-------------------------------------------------

QT       += core gui widgets core5compat

TARGET = SubtitlesFixer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

SOURCES += \
    src/main.cpp \
    src/subtitles_fixer_mainwindow.cpp \

INCLUDEPATH += \
        include \
        ../SubtitleFixerLib/include

HEADERS += \
    include/subtitles_fixer_mainwindow.h \


FORMS += \
        subtitles_fixer_mainwindow.ui


win32:CONFIG(release, debug|release): LIBS += -L../SubtitleFixerLib/release/ -lSubtitleFixerLib
else:win32:CONFIG(debug, debug|release): LIBS += -L../SubtitleFixerLib/debug/ -lSubtitleFixerLib
else:unix: LIBS += -L../SubtitleFixerLib/ -lSubtitleFixerLib


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target