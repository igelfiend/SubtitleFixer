#ifndef TEST_SUBTITLE_VALIDATOR_RULE_H
#define TEST_SUBTITLE_VALIDATOR_RULE_H

#include <QtTest>
#include <QString>

#include "auto_test.h"
#include "reader/subtitle_block.h"
#include "editor/subtitle_style_rule.h"


class TestBlockValidatorRule: public QObject
{
    Q_OBJECT

public:
    TestBlockValidatorRule();
    ~TestBlockValidatorRule() { }

private slots:
    void TestHasHeaderRule_data();
    void TestHasHeaderRule();

    void TestHasFormatRowRule_data();
    void TestHasFormatRowRule();

    void TestColumnsCountEqualRule_data();
    void TestColumnsCountEqualRule();

private:
    SubtitleBlock regularBlock;
    SubtitleBlock emptyBlock;
    SubtitleBlock onlyWithTitleBlock;
};

inline TestBlockValidatorRule::TestBlockValidatorRule()
{
    regularBlock = SubtitleBlock{
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
    emptyBlock = SubtitleBlock{
        "",
        QList< SubtitleRowPtr >{ }
    };
    onlyWithTitleBlock = SubtitleBlock{
        "TestBlock",
        QList< SubtitleRowPtr >{
            SubtitleRowPtr( new HeaderSubtitleRow( "[TestBlock]" ) ),
        }
    };
}

inline void TestBlockValidatorRule::TestHasHeaderRule_data()
{
    QTest::addColumn< SubtitleBlock >( "block" );
    QTest::addColumn< bool >( "expected" );

    auto headlessBlock = SubtitleBlock{
        "TestBlock",
        QList< SubtitleRowPtr >{
            SubtitleRowPtr( new FormatterSubtitleRow( "Format: A, B, C" ) ),
            SubtitleRowPtr( new ContentSubtitleRow  ( "Style: Bold, 20, Times New Romans" ) ),
            SubtitleRowPtr( new ContentSubtitleRow  ( "Style: Italic, 50, Terminal" ) ),
        }
    };
    auto anonymousBlock = SubtitleBlock{
        "",
        QList< SubtitleRowPtr >{
            SubtitleRowPtr( new HeaderSubtitleRow   ( "[]" ) ),
            SubtitleRowPtr( new FormatterSubtitleRow( "Format: A, B, C" ) ),
            SubtitleRowPtr( new ContentSubtitleRow  ( "Style: Bold, 20, Times New Romans" ) ),
            SubtitleRowPtr( new ContentSubtitleRow  ( "Style: Italic, 50, Terminal" ) ),
        }
    };

    QTest::newRow( "block with regular header is valid" ) << this->regularBlock       << true;
    QTest::newRow( "block with only header is valid" )    << this->onlyWithTitleBlock << true;
    QTest::newRow( "empty block is not valid" )           << this->emptyBlock         << false;
    QTest::newRow( "headless block is not valid" )        << headlessBlock            << false;
    QTest::newRow( "anonymous block is not valid" )       << anonymousBlock           << false;
}

inline void TestBlockValidatorRule::TestHasHeaderRule()
{
    QFETCH(SubtitleBlock, block);
    QFETCH(bool, expected);

    QVERIFY( HasHeaderRule(block).validate() == expected );
}

inline void TestBlockValidatorRule::TestHasFormatRowRule_data()
{
    QTest::addColumn< SubtitleBlock >( "block" );
    QTest::addColumn< bool >( "expected" );

    auto blockWithoutFormatter = SubtitleBlock{
        "TestBlock",
        QList< SubtitleRowPtr >{
            SubtitleRowPtr( new HeaderSubtitleRow   ( "[TestBlock]" ) ),
            SubtitleRowPtr( new ContentSubtitleRow  ( "Style: Bold, 20, Times New Romans" ) ),
        }
    };

    QTest::newRow( "regular block with format row is valid" )   << this->regularBlock    << true;
    QTest::newRow( "block without format row is not valid" )    << blockWithoutFormatter << false;
}

inline void TestBlockValidatorRule::TestHasFormatRowRule()
{
    QFETCH(SubtitleBlock, block);
    QFETCH(bool, expected);

    QVERIFY( HasFormatRowRule(block).validate() == expected );
}

inline void TestBlockValidatorRule::TestColumnsCountEqualRule_data()
{
    QTest::addColumn< SubtitleBlock >( "block" );
    QTest::addColumn< bool >( "expected" );

    auto blockWithDifferentElementsCount = SubtitleBlock{
        "TestBlock",
        QList< SubtitleRowPtr >{
            SubtitleRowPtr( new HeaderSubtitleRow   ( "[TestBlock]" ) ),
            SubtitleRowPtr( new FormatterSubtitleRow( "Format: A, B" ) ),
            SubtitleRowPtr( new ContentSubtitleRow  ( "Style: Bold, 20, Times New Romans" ) ),
            SubtitleRowPtr( new ContentSubtitleRow  ( "Style: Italic, 50" ) ),
        }
    };

    QTest::newRow( "block with equal count of elements is valid" ) << this->regularBlock       << true;
    QTest::newRow( "block with only header is valid" )             << this->onlyWithTitleBlock << true;
    QTest::newRow( "empty block is valid" )                        << this->emptyBlock         << true;
    QTest::newRow( "block with different elements count is not valid" )
            << blockWithDifferentElementsCount
            << false;
}

inline void TestBlockValidatorRule::TestColumnsCountEqualRule()
{
    QFETCH(SubtitleBlock, block);
    QFETCH(bool, expected);

    QVERIFY( ColumnsCountEqualRule(block).validate() == expected );
}

DECLARE_TEST(TestBlockValidatorRule)

#endif // TEST_SUBTITLE_VALIDATOR_RULE_H
