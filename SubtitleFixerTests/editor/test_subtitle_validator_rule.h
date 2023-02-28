#ifndef TEST_SUBTITLE_VALIDATOR_RULE_H
#define TEST_SUBTITLE_VALIDATOR_RULE_H

#include <QtTest>
#include <QString>

#include "auto_test.h"
#include "reader/subtitle_block.h"
#include "editor/subtitle_block_rule.h"

static constexpr bool CREATE_TITLE = true;
static constexpr bool DONT_CREATE_TITLE = false;
static constexpr bool CREATE_FORMAT_ROW = true;
static constexpr bool DONT_CREATE_FORMAT_ROW = false;

class TestBlockValidatorRule: public QObject
{
    Q_OBJECT

public:
    ~TestBlockValidatorRule() { }

private slots:
    void initTestCase();

    void TestHasHeaderRule_data();
    void TestHasHeaderRule();

    void TestHasFormatRowRule_data();
    void TestHasFormatRowRule();

    void TestColumnsCountEqualRule_data();
    void TestColumnsCountEqualRule();

private:
    QList< SubtitleRowPtr > createRows(bool createTitle, bool createFormatRow) const;

    SubtitleBlock regularStyleBlock;
    SubtitleBlock regularEventsBlock;
    SubtitleBlock regularOtherBlock;
    SubtitleBlock emptyBlock;
    SubtitleBlock onlyWithTitleBlock;
};

inline QList<SubtitleRowPtr> TestBlockValidatorRule::createRows(bool createTitle, bool createFormatRow) const
{
    QList< SubtitleRowPtr > result{};
    if (createTitle)
    {
        result.append( QSharedPointer<HeaderSubtitleRow>::create( "[TestBlock]" ) );
    }
    if (createFormatRow)
    {
        result.append( QSharedPointer<FormatterSubtitleRow>::create( "Format: A, B, C" ) );
    }

    result << QList< SubtitleRowPtr >{
        SubtitleRowPtr( new CommentSubtitleRow  ( "; This is comment row" ) ),
        SubtitleRowPtr( new ContentSubtitleRow  ( "Style: Bold, 20, Times New Romans" ) ),
        SubtitleRowPtr( new ContentSubtitleRow  ( "Style: Italic, 50, Terminal" ) ),
        SubtitleRowPtr( new UnknownSubtitleRow  ( "Row with unrecognized data" ) ),
        SubtitleRowPtr( new EmptySubtitleRow() ),
    };

    return result;
}

void TestBlockValidatorRule::initTestCase()
{
    emptyBlock = SubtitleBlock{
        "",
        QList< SubtitleRowPtr >{ },
        SubtitleBlock::BlockType::OTHER_BLOCK,
    };
    onlyWithTitleBlock = SubtitleBlock{
        "TestBlock",
        QList< SubtitleRowPtr >{
            SubtitleRowPtr( new HeaderSubtitleRow( "[TestBlock]" ) ),
        },
        SubtitleBlock::BlockType::OTHER_BLOCK,
    };
}

inline void TestBlockValidatorRule::TestHasHeaderRule_data()
{
    auto createBlock = [this](bool createTitle, SubtitleBlock::BlockType blockType)
    {
        return SubtitleBlock{
            "TestBlock",
            createRows(createTitle, CREATE_FORMAT_ROW),
            blockType,
        };
    };

    QTest::addColumn< SubtitleBlock >( "block" );
    QTest::addColumn< bool >( "expected" );

    auto regularStyleBlock  = createBlock( CREATE_TITLE, SubtitleBlock::BlockType::STYLE_BLOCK  );
    auto regularEventsBlock = createBlock( CREATE_TITLE, SubtitleBlock::BlockType::EVENTS_BLOCK );
    auto regularOtherBlock  = createBlock( CREATE_TITLE, SubtitleBlock::BlockType::OTHER_BLOCK  );

    auto headlessStyleBlock  = createBlock( DONT_CREATE_TITLE, SubtitleBlock::BlockType::STYLE_BLOCK  );
    auto headlessEventsBlock = createBlock( DONT_CREATE_TITLE, SubtitleBlock::BlockType::EVENTS_BLOCK );
    auto headlessOtherBlock  = createBlock( DONT_CREATE_TITLE, SubtitleBlock::BlockType::OTHER_BLOCK  );

    auto anonymousBlock = SubtitleBlock{
        "",
        QList< SubtitleRowPtr >{
            SubtitleRowPtr( new HeaderSubtitleRow   ( "[]" ) ),
            SubtitleRowPtr( new FormatterSubtitleRow( "Format: A, B, C" ) ),
            SubtitleRowPtr( new ContentSubtitleRow  ( "Style: Bold, 20, Times New Romans" ) ),
            SubtitleRowPtr( new ContentSubtitleRow  ( "Style: Italic, 50, Terminal" ) ),
        }
    };

    QTest::newRow( "style block with regular header is valid"  ) << regularStyleBlock  << true;
    QTest::newRow( "events block with regular header is valid" ) << regularEventsBlock << true;
    QTest::newRow( "other block with regular header is valid"  ) << regularOtherBlock  << true;
    QTest::newRow( "block with only header is valid" )    << this->onlyWithTitleBlock << true;
    QTest::newRow( "headless style block is not valid"  ) << headlessStyleBlock << false;
    QTest::newRow( "headless events block is not valid" ) << headlessStyleBlock << false;
    QTest::newRow( "headless other block is not valid"  ) << headlessStyleBlock << false;
    QTest::newRow( "anonymous block is not valid" )       << anonymousBlock     << false;
    QTest::newRow( "empty block is not valid" )           << this->emptyBlock   << false;
}

inline void TestBlockValidatorRule::TestHasHeaderRule()
{
    QFETCH(SubtitleBlock, block);
    QFETCH(bool, expected);

    QVERIFY( HasHeaderRule().validate(block) == expected );
}

inline void TestBlockValidatorRule::TestHasFormatRowRule_data()
{
    auto createBlock = [this](bool createFormatRow, SubtitleBlock::BlockType blockType)
    {
        return SubtitleBlock{
            "TestBlock",
            createRows(CREATE_TITLE, createFormatRow),
            blockType,
        };
    };
    auto createBlockWithMultipleFormatRows = [](SubtitleBlock::BlockType blockType)
    {
        return SubtitleBlock{
            "TestBlock",
            QList< SubtitleRowPtr >{
                SubtitleRowPtr( new HeaderSubtitleRow   ( "[TestBlock]" ) ),
                SubtitleRowPtr( new FormatterSubtitleRow( "Format: A, B" ) ),
                SubtitleRowPtr( new FormatterSubtitleRow( "Format: C, D" ) ),
                SubtitleRowPtr( new ContentSubtitleRow  ( "Style: Bold, 20, Times New Romans" ) ),
            },
            blockType,
        };
    };

    QTest::addColumn< SubtitleBlock >( "block" );
    QTest::addColumn< bool >( "expected" );

    auto styleBlockWithFormatter  = createBlock( CREATE_FORMAT_ROW, SubtitleBlock::BlockType::STYLE_BLOCK  );
    auto eventsBlockWithFormatter = createBlock( CREATE_FORMAT_ROW, SubtitleBlock::BlockType::EVENTS_BLOCK );
    auto otherBlockWithFormatter  = createBlock( CREATE_FORMAT_ROW, SubtitleBlock::BlockType::OTHER_BLOCK  );

    auto styleBlockWithoutFormatter  = createBlock( DONT_CREATE_FORMAT_ROW, SubtitleBlock::BlockType::STYLE_BLOCK  );
    auto eventsBlockWithoutFormatter = createBlock( DONT_CREATE_FORMAT_ROW, SubtitleBlock::BlockType::EVENTS_BLOCK );
    auto otherBlockWithoutFormatter  = createBlock( DONT_CREATE_FORMAT_ROW, SubtitleBlock::BlockType::OTHER_BLOCK  );

    auto styleBlockWithMultipleFormatters  = createBlockWithMultipleFormatRows( SubtitleBlock::STYLE_BLOCK  );
    auto eventsBlockWithMultipleFormatters = createBlockWithMultipleFormatRows( SubtitleBlock::EVENTS_BLOCK );
    auto otherBlockWithMultipleFormatters  = createBlockWithMultipleFormatRows( SubtitleBlock::OTHER_BLOCK  );


    QTest::newRow( "regular style block with format row is valid"  ) << styleBlockWithFormatter  << true;
    QTest::newRow( "regular events block with format row is valid" ) << eventsBlockWithFormatter << true;
    QTest::newRow( "regular other block with format row is valid"  ) << otherBlockWithFormatter  << true;

    QTest::newRow( "regular style block without format row is invalid"  ) << styleBlockWithoutFormatter  << false;
    QTest::newRow( "regular events block without format row is invalid" ) << eventsBlockWithoutFormatter << false;
    QTest::newRow( "regular other block without format row is valid"    ) << otherBlockWithoutFormatter  << true;

    QTest::newRow( "style block with multiple format rows is invalid"   ) << styleBlockWithMultipleFormatters  << false;
    QTest::newRow( "events block with multiple format rows is invalid"  ) << eventsBlockWithMultipleFormatters << false;
    QTest::newRow( "other block with multiple format rows is valid"     ) << otherBlockWithMultipleFormatters  << true;
}

inline void TestBlockValidatorRule::TestHasFormatRowRule()
{
    QFETCH(SubtitleBlock, block);
    QFETCH(bool, expected);

    QVERIFY( HasOneFormatRowRule().validate(block) == expected );
}

inline void TestBlockValidatorRule::TestColumnsCountEqualRule_data()
{
    auto createRegularBlock = [this](SubtitleBlock::BlockType blockType)
    {
        return SubtitleBlock{
            "TestBlock",
            createRows(CREATE_TITLE, CREATE_FORMAT_ROW),
            blockType,
        };
    };
    auto createBlockWithDifferentElementsCount = [](SubtitleBlock::BlockType blockType)
    {
        return SubtitleBlock{
            "TestBlock",
            QList< SubtitleRowPtr >{
                SubtitleRowPtr( new HeaderSubtitleRow   ( "[TestBlock]" ) ),
                SubtitleRowPtr( new FormatterSubtitleRow( "Format: A, B" ) ),
                SubtitleRowPtr( new ContentSubtitleRow  ( "Style: Bold, 20, Times New Romans" ) ),
                SubtitleRowPtr( new ContentSubtitleRow  ( "Style: Italic, 50" ) ),
            },
            blockType,
        };
    };

    QTest::addColumn< SubtitleBlock >( "block" );
    QTest::addColumn< bool >( "expected" );

    auto regularStyleBlock  = createRegularBlock( SubtitleBlock::STYLE_BLOCK  );
    auto regularEventsBlock = createRegularBlock( SubtitleBlock::EVENTS_BLOCK );
    auto regularOtherBlock  = createRegularBlock( SubtitleBlock::OTHER_BLOCK  );

    auto styleBlockWithDifferentElementsCount  = createBlockWithDifferentElementsCount( SubtitleBlock::STYLE_BLOCK  );
    auto eventsBlockWithDifferentElementsCount = createBlockWithDifferentElementsCount( SubtitleBlock::EVENTS_BLOCK );
    auto otherBlockWithDifferentElementsCount  = createBlockWithDifferentElementsCount( SubtitleBlock::OTHER_BLOCK  );

    QTest::newRow( "style block with equal count of elements is valid"  ) << regularStyleBlock  << true;
    QTest::newRow( "events block with equal count of elements is valid" ) << regularEventsBlock << true;
    QTest::newRow( "other block with equal count of elements is valid"  ) << regularOtherBlock  << true;
    QTest::newRow( "style block with different elements count is not valid" )
            << styleBlockWithDifferentElementsCount
            << false;
    QTest::newRow( "events block with different elements count is not valid" )
            << eventsBlockWithDifferentElementsCount
            << false;
    QTest::newRow( "other block with different elements count is not valid" )
            << otherBlockWithDifferentElementsCount
            << false;
    QTest::newRow( "block with only header is valid" ) << this->onlyWithTitleBlock << true;
    QTest::newRow( "empty block is valid" )            << this->emptyBlock         << true;
}

inline void TestBlockValidatorRule::TestColumnsCountEqualRule()
{
    QFETCH(SubtitleBlock, block);
    QFETCH(bool, expected);

    QVERIFY( ColumnsCountEqualRule().validate(block) == expected );
}

DECLARE_TEST(TestBlockValidatorRule)

#endif // TEST_SUBTITLE_VALIDATOR_RULE_H
