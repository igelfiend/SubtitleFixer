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

CONFIG += c++11

SOURCES += \
    src/reader/subtitle_row_serializer.cpp \
    src/editor/subtitle_style_editor.cpp \
    src/main.cpp \
    src/subtitles_fixer_mainwindow.cpp \
    src/subtitles_fixer.cpp

INCLUDEPATH += include

HEADERS += \
    include/editor/subtitle_style_editor.h \
    include/editor/subtitle_style_rule.h \
    include/editor/subtitle_style_validator.h \
    include/codec_settings.h \
    include/option.h \
    include/reader/subtitle_block_reader.h \
    include/reader/subtitle_data.h \
    include/reader/subtitle_row.h \
    include/reader/subtitle_row_serializer.h \
    include/settings.h \
    include/reader/subtitle_block.h \
    include/string_helper.h \
    include/subtitles_fixer_mainwindow.h \
    include/subtitles_fixer_exceptions.h \
    include/subtitles_fixer.h


FORMS += \
        subtitles_fixer_mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
