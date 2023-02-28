QT += testlib core5compat
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG += c++20
CONFIG -= app_bundle

DEFINES += Q_COMPILER_INITIALIZER_LISTS

TEMPLATE = app

SOURCES +=  \
        main.cpp

INCLUDEPATH += \
        ../SubtitleFixerLib/include


win32:CONFIG(release, debug|release): LIBS += -L../SubtitleFixerLib/release/ -lSubtitleFixerLib
else:win32:CONFIG(debug, debug|release): LIBS += -L../SubtitleFixerLib/debug/ -lSubtitleFixerLib
else:unix: LIBS += -L../SubtitleFixerLib/ -lSubtitleFixerLib

HEADERS += \
    auto_test.h\
    auto_test_utils.h \
    editor/test_subtitle_block_editor.h \
    fixtures/fixture_subtitle.h \
    reader/test_subtitle_data_serializer.h \
    reader/test_subtitle_block.h \
    reader/test_subtitle_row.h \
    editor/test_subtitle_validator.h \
    editor/test_subtitle_validator_rule.h \
    test_string_helpers.h \
    test_subtitles_fixer.h
