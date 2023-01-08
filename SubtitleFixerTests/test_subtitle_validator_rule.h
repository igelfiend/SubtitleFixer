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
};

inline TestBlockValidatorRule::TestBlockValidatorRule()
{
    regularBlock = SubtitleBlock{
        "TestBlock",
        QList<SubtitleRow>{
            HeaderSubtitleRow   ( "[TestBlock]" ),
            FormatterSubtitleRow( "Format: A, B, C" ),
            ContentSubtitleRow  ( "Style: Bold, 20, Times New Romans" ),
            ContentSubtitleRow  ( "Style: Italic, 50, Terminal" ),
        }
    };
    auto emptyBlock = SubtitleBlock{
        "",
        QList<SubtitleRow>{ }
    };
}

inline void TestBlockValidatorRule::TestHasHeaderRule_data()
{
    QTest::addColumn< SubtitleBlock >( "block" );
    QTest::addColumn< bool >( "expected" );

    auto headlessBlock = SubtitleBlock{
        "TestBlock",
        QList<SubtitleRow>{
            FormatterSubtitleRow( "Format: A, B, C" ),
            ContentSubtitleRow  ( "Style: Bold, 20, Times New Romans" ),
            ContentSubtitleRow  ( "Style: Italic, 50, Terminal" ),
        }
    };
    auto anonymousBlock = SubtitleBlock{
        "",
        QList<SubtitleRow>{
            HeaderSubtitleRow   ( "[]" ),
            FormatterSubtitleRow( "Format: A, B, C" ),
            ContentSubtitleRow  ( "Style: Bold, 20, Times New Romans" ),
            ContentSubtitleRow  ( "Style: Italic, 50, Terminal" ),
        }
    };

    QTest::newRow( "block with regular header is valid" ) << this->regularBlock << true;
    QTest::newRow( "empty block is not valid" )           << this->emptyBlock   << false;
    QTest::newRow( "headless block is not valid" )        << headlessBlock      << false;
    QTest::newRow( "anonymous block is not valid" )       << anonymousBlock     << false;
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
        QList<SubtitleRow>{
            HeaderSubtitleRow   ( "[TestBlock]" ),
            ContentSubtitleRow  ( "Style: Bold, 20, Times New Romans" ),
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
        QList<SubtitleRow>{
            HeaderSubtitleRow   ( "[TestBlock]" ),
            FormatterSubtitleRow( "Format: A, B" ),
            ContentSubtitleRow  ( "Style: Bold, 20, Times New Romans" ),
            ContentSubtitleRow  ( "Style: Italic, 50" ),
        }
    };

    QTest::newRow( "block with equal count of elements is valid" ) << this->regularBlock << true;
    QTest::newRow( "empty block is not valid" )                    << this->emptyBlock   << false;
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
