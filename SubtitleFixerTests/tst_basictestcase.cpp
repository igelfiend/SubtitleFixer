#include <QtTest>

// add necessary includes here

class BasicTestCase : public QObject
{
    Q_OBJECT

public:
    BasicTestCase();
    ~BasicTestCase();

private slots:
    void test_case1();

};

BasicTestCase::BasicTestCase()
{

}

BasicTestCase::~BasicTestCase()
{

}

void BasicTestCase::test_case1()
{

}

QTEST_APPLESS_MAIN(BasicTestCase)

#include "tst_basictestcase.moc"
