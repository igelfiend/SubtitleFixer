#ifndef TEST_SUBTITLE_VALIDATOR_H
#define TEST_SUBTITLE_VALIDATOR_H

#include <QtTest>
#include <QString>

#include "auto_test.h"
#include "editor/subtitle_block_rule.h"
#include "editor/subtitle_block_validator.h"

class TestBlockValidator: public QObject
{
    Q_OBJECT

public:
    TestBlockValidator() { }
    ~TestBlockValidator() { }

private slots:
    void TestValidate_data();
    void TestValidate();

private:
    SubtitleBlock dummyBlock;
};

class AlwaysInvalidRule: public AbstractSubtitleBlockValidationRule
{
public:
    bool validate(const SubtitleBlock&) const override { return false; }
    QString getError() const override { return "always invalid rule"; }
};

class AlwaysValidRule: public AbstractSubtitleBlockValidationRule
{
public:
    bool validate(const SubtitleBlock&) const override { return true; }
    QString getError() const override { return "always valid rule"; }
};

inline void TestBlockValidator::TestValidate_data()
{
    QTest::addColumn< QList< RuleConstPtr > >( "rules" );
    QTest::addColumn< bool >( "expected_validation_result" );
    QTest::addColumn< QStringList >( "expected_errors" );

    QList< RuleConstPtr > validRules{
        QSharedPointer< const AlwaysValidRule >::create(),
        QSharedPointer< const AlwaysValidRule >::create(),
    };
    QList< RuleConstPtr > invalidRules{
        QSharedPointer< const AlwaysInvalidRule >::create(),
        QSharedPointer< const AlwaysInvalidRule >::create(),
    };
    QList< RuleConstPtr > mixedRules{
        QSharedPointer< const AlwaysInvalidRule >::create(),
        QSharedPointer< const AlwaysValidRule   >::create(),
    };

    QTest::addRow( "valid if all rules are valid" )
            << validRules
            << true
            << QStringList();
    QTest::addRow( "invalid if all rules are invalid" )
            << invalidRules
            << false
            << QStringList{ "always invalid rule", "always invalid rule" };
    QTest::addRow( "invalid if some rules are valid and some are not" )
            << mixedRules
            << false
            << QStringList{ "always invalid rule" };
}

inline void TestBlockValidator::TestValidate()
{
    QFETCH( QList< RuleConstPtr >, rules );
    QFETCH( bool, expected_validation_result);
    QFETCH( QStringList, expected_errors );

    SubtitleBlockValidator validator;
    validator.setRules( rules );
    bool validation_result = validator.validate( dummyBlock );

    QVERIFY( expected_validation_result == validation_result );
    QCOMPARE_EQ( expected_errors, validator.getErrors() );
}


DECLARE_TEST(TestBlockValidator)

#endif // TEST_SUBTITLE_VALIDATOR_H
