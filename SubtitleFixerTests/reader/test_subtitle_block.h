#ifndef TEST_SUBTITLE_BLOCK_H
#define TEST_SUBTITLE_BLOCK_H

#include <QtTest>
#include <QString>

#include "auto_test.h"
#include "reader/subtitle_block.h"
#include "reader/subtitle_row.h"
#include "subtitles_fixer_exceptions.h"

class TestSubtitleBlock: public QObject
{
    Q_OBJECT
public:
    TestSubtitleBlock()
    {
        blockWithFormatter = SubtitleBlock{
            "TestBlock",
            QList< SubtitleRowPtr >{
                SubtitleRowPtr( new HeaderSubtitleRow   ( "[TestBlock]" ) ),
                SubtitleRowPtr( new CommentSubtitleRow  ( "; This is comment row" ) ),
                SubtitleRowPtr( new FormatterSubtitleRow( "Format: A, B, C" ) ),
                SubtitleRowPtr( new ContentSubtitleRow  ( "Style: Bold, 20, Times New Romans" ) ),
                SubtitleRowPtr( new ContentSubtitleRow  ( "Style: Italic, 50, Terminal" ) ),
                SubtitleRowPtr( new UnknownSubtitleRow  ( "Row with unrecognized data" ) ),
                SubtitleRowPtr( new EmptySubtitleRow() ),
            }
        };
        blockWithoutFormatter = SubtitleBlock{
            "TestBlock",
            QList< SubtitleRowPtr >{
                SubtitleRowPtr( new HeaderSubtitleRow   ( "[TestBlock]" ) ),
                SubtitleRowPtr( new CommentSubtitleRow  ( "; This is comment row" ) ),
                SubtitleRowPtr( new ContentSubtitleRow  ( "Style: Bold, 20, Times New Romans" ) ),
                SubtitleRowPtr( new ContentSubtitleRow  ( "Style: Italic, 50, Terminal" ) ),
                SubtitleRowPtr( new UnknownSubtitleRow  ( "Row with unrecognized data" ) ),
                SubtitleRowPtr( new EmptySubtitleRow() ),
            }
        };
    }

    ~TestSubtitleBlock() = default;

private slots:
    void TestGetFormatLine();
    void TestGetFormatLineRaiseExceptionIfNotFound();

private:
    SubtitleBlock blockWithFormatter;
    SubtitleBlock blockWithoutFormatter;
};

inline void TestSubtitleBlock::TestGetFormatLine()
{
    SubtitleRowPtr formatLine = blockWithFormatter.getFormatLine();

    QCOMPARE_EQ( formatLine, blockWithFormatter.lines[2] );
}

inline void TestSubtitleBlock::TestGetFormatLineRaiseExceptionIfNotFound()
{
    QVERIFY_THROWS_EXCEPTION(FormatLineNotFoundException, blockWithoutFormatter.getFormatLine());
}

DECLARE_TEST( TestSubtitleBlock )

#endif // TEST_SUBTITLE_BLOCK_H
