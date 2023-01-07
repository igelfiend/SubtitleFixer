QT -= gui
QT += core5compat

TARGET = SubtitleFixerLib

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/reader/subtitle_row_serializer.cpp \
    src/editor/subtitle_style_editor.cpp \
    src/subtitles_fixer.cpp

INCLUDEPATH += include

HEADERS += \
    include/editor/subtitle_style_editor.h \
    include/editor/subtitle_style_rule.h \
    include/editor/subtitle_style_validator.h \
    include/codec_settings.h \
    include/option.h \
    include/reader/subtitle_block.h \
    include/reader/subtitle_block_reader.h \
    include/reader/subtitle_data.h \
    include/reader/subtitle_row.h \
    include/reader/subtitle_row_serializer.h \
    include/settings.h \
    include/string_helper.h \
    include/subtitles_fixer_exceptions.h \
    include/subtitles_fixer.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
