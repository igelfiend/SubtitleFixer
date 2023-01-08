#ifndef TEST_STRING_HELPERS_H
#define TEST_STRING_HELPERS_H

#include <QtTest>
#include <QDebug>
#include <QString>

#include "auto_test.h"
#include "auto_test_utils.h"
#include "string_helper.h"

class TestStringHelpers : public QObject
{
    Q_OBJECT

public:
    TestStringHelpers() { }
    ~TestStringHelpers() { }

private slots:
    void TestStringIsBlockHeader_data();
    void TestStringIsBlockHeader();

    void TestExtractBlockHeader_data();
    void TestExtractBlockHeader();

    void TestGetRowDataFromString_data();
    void TestGetRowDataFromString();
    void TestGetRowDataFromStringInvalidStringMissedColon();

};

inline void TestStringHelpers::TestStringIsBlockHeader_data()
{
    QTest::addColumn< QString >( "string" );
    QTest::addColumn< bool    >( "expected" );

    QTest::newRow( "normal header with filled body"  ) << "[SomeBlock]"  << true;
    QTest::newRow( "normal header with empty body"   ) << "[]"           << true;
    QTest::newRow( "normal header with extra spaces" ) << " [header]\n"  << true;

    QTest::newRow( "incorrect header without brackets"   ) << "SomeBlock" << false;
    QTest::newRow( "incorrect header with start bracket" ) << "[SomeBlock" << false;
    QTest::newRow( "incorrect header with end bracket"   ) << "SomeBlock]" << false;
    QTest::newRow( "incorrect header as empty row"       ) << ""           << false;
}

void TestStringHelpers::TestStringIsBlockHeader()
{
    QFETCH(QString, string);
    QFETCH(bool, expected);

    QVERIFY(stringIsBlockHeader(string) == expected);
}

inline void TestStringHelpers::TestExtractBlockHeader_data()
{
    QTest::addColumn< QString >( "string" );
    QTest::addColumn< QString >( "expected" );

    QTest::newRow( "normal header with filled body"  ) << "[SomeBlock]"  << "SomeBlock";
    QTest::newRow( "normal header with empty body"   ) << "[]"           << "";
    QTest::newRow( "normal header with extra spaces" ) << " [header]\n"  << "header";
}

inline void TestStringHelpers::TestExtractBlockHeader()
{
    QFETCH(QString, string);
    QFETCH(QString, expected);

    QCOMPARE(extractBlockHeader(string), expected);
}

inline void TestStringHelpers::TestGetRowDataFromString_data()
{
    QTest::addColumn< QString >( "string" );
    QTest::addColumn< RowData >( "expected" );

    QTest::newRow( "standard event string" )
            << "Event: 10,20,30,Hello"
            << RowData{"Event", QStringList{"10", "20", "30", "Hello"}};
    QTest::newRow( "standard formatter string" )
            << "Format: Font, Size, Hello"
            << RowData{"Format", QStringList{"Font", "Size", "Hello"}};
    QTest::newRow( "string with different space cases" )
            << "Hello:Font,    Size,world"
            << RowData{"Hello", QStringList{"Font", "Size", "world"}};
}

inline void TestStringHelpers::TestGetRowDataFromString()
{
    QFETCH(QString, string);
    QFETCH(RowData, expected);

    RowData result = getRowDataFromString(string);
    QCOMPARE(result.title,  expected.title);
    QCOMPARE(result.values, expected.values);
}

inline void TestStringHelpers::TestGetRowDataFromStringInvalidStringMissedColon()
{

    QString str = "Event - Hello, World";

    qInstallMessageHandler(noMessageOutput);
    QVERIFY_THROWS_EXCEPTION(std::runtime_error, getRowDataFromString(str));
    qInstallMessageHandler(nullptr);
}


DECLARE_TEST(TestStringHelpers)

#endif //TEST_STRING_HELPERS_H
