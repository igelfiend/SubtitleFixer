#ifndef TEST_SUBTITLE_BLOCK_EDITOR_H
#define TEST_SUBTITLE_BLOCK_EDITOR_H

#include <QtTest>
#include <QDebug>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QStringList>

#include "editor/subtitle_block_editor.h"
#include "reader/subtitle_block_reader.h"
#include "reader/subtitle_block.h"
#include "reader/subtitle_row.h"
#include "auto_test.h"

class TestSubtitleBlockEditor : public QObject
{
    Q_OBJECT

public:
    TestSubtitleBlockEditor() { }
    ~TestSubtitleBlockEditor() { }

private slots:
    void initTestCase();
    void TestGetFormatterFields();
    void TestUpdateAllValuesForField();

private:
    SubtitleBlock testBlock;
    SubtitleBlockReader blockReader;
};

inline void TestSubtitleBlockEditor::initTestCase()
{
    testBlock = blockReader.readBlock( { "[Events]",
                                          "Format: Layer, Name, Text",
                                          "Dialogue: 0,Main,Hello World",
                                          "Dialogue: 1,Dialog,Hey there" } );
}

inline void TestSubtitleBlockEditor::TestGetFormatterFields()
{
    SubtitleBlock block = testBlock;
    SubtitleBlockEditor blockEditor( block );

    QStringList expected{ "Layer", "Name", "Text" };
    QCOMPARE( expected, blockEditor.getFormatterFields() );
}

inline void TestSubtitleBlockEditor::TestUpdateAllValuesForField()
{
    SubtitleBlock block = testBlock;
    SubtitleBlockEditor blockEditor( block );

    blockEditor.updateAllValuesForField(
                "Layer",
                [](const QString &){ return "2"; }
    );

    auto dataLines = block.getDataLines();
    QCOMPARE( dataLines[0]->getValues()[0], "2" );
    QCOMPARE( dataLines[1]->getValues()[0], "2" );
}

DECLARE_TEST(TestSubtitleBlockEditor)

#endif // TEST_SUBTITLE_BLOCK_EDITOR_H
