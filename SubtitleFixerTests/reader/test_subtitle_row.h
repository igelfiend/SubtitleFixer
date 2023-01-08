#ifndef TEST_SUBTITLE_ROW_H
#define TEST_SUBTITLE_ROW_H

#include <QtTest>
#include <QDebug>
#include <QString>

#include "auto_test.h"
#include "auto_test_utils.h"
#include "reader/subtitle_row.h"

class TestSubtitleRow: public QObject
{
    Q_OBJECT
public:
    TestSubtitleRow()  { }
    ~TestSubtitleRow() { }

private slots:
    void TestCommentSubtitleRow();
    void TestHeaderSubtitleRow();
    void TestContentSubtitleRow();
    void TestFormatterSubtitleRow();
    void TestEmptySubtitleRow();
    void TestUnknownSubtitleRow();
};

inline void TestSubtitleRow::TestCommentSubtitleRow()
{
    QString row = ";comment row";

    CommentSubtitleRow subRow( row );

    QCOMPARE_EQ( subRow.getTitle(), "comment row" );
    QCOMPARE_EQ( subRow.isStoreValues(), false );
    QCOMPARE_EQ( subRow.toString(), row );
}

inline void TestSubtitleRow::TestHeaderSubtitleRow()
{
    QString row = "[TestHeader]";

    HeaderSubtitleRow subRow( row );

    QCOMPARE_EQ( subRow.getTitle(), "TestHeader" );
    QCOMPARE_EQ( subRow.isStoreValues(), false );
    QCOMPARE_EQ( subRow.toString(), row );
}

inline void TestSubtitleRow::TestContentSubtitleRow()
{
    QString row = "Style: Main,Calibri,42,,,&H00FFFFFF";

    ContentSubtitleRow subRow( row );

    QStringList expectedValues{ "Main", "Calibri", "42", "", "", "&H00FFFFFF" };
    QCOMPARE_EQ( subRow.getTitle(), "Style" );
    QCOMPARE_EQ( subRow.getValues(), expectedValues );
    QCOMPARE_EQ( subRow.isStoreValues(), true );
    QCOMPARE_EQ( subRow.toString(), row );
}

inline void TestSubtitleRow::TestFormatterSubtitleRow()
{
    QString row = "Format: Name, Fontname, Fontsize";

    FormatterSubtitleRow subRow( row );

    QStringList expectedValues{ "Name", "Fontname", "Fontsize" };
    QCOMPARE_EQ( subRow.getTitle(), "Format" );
    QCOMPARE_EQ( subRow.getValues(), expectedValues );
    QCOMPARE_EQ( subRow.isStoreValues(), true );
    QCOMPARE_EQ( subRow.toString(), row );
}

inline void TestSubtitleRow::TestEmptySubtitleRow()
{
    EmptySubtitleRow subRow;

    QCOMPARE_EQ( subRow.getTitle(), "" );
    QCOMPARE_EQ( subRow.isStoreValues(), false );
    QCOMPARE_EQ( subRow.toString(), "" );
}

inline void TestSubtitleRow::TestUnknownSubtitleRow()
{
    QString row = "-- Some unknown kind of row --";

    UnknownSubtitleRow subRow( row );

    QCOMPARE_EQ( subRow.getTitle(), row );
    QCOMPARE_EQ( subRow.isStoreValues(), false );
    QCOMPARE_EQ( subRow.toString(), row );
}


DECLARE_TEST( TestSubtitleRow )


#endif // TEST_SUBTITLE_ROW_H
