#ifndef SUBTITLE_BLOCK_VALIDATOR_H
#define SUBTITLE_BLOCK_VALIDATOR_H

#include <QList>
#include <QStringList>
#include <QSharedPointer>

#include "reader/subtitle_block.h"
#include "subtitle_block_rule.h"


class ISubtitleBlockValidator
{
public:
    virtual ~ISubtitleBlockValidator() { }
    virtual bool validate(const SubtitleBlock &block) = 0;
    virtual void setRules(const QList<RuleConstPtr>&) = 0;
    virtual void addRules(const QList<RuleConstPtr>&) = 0;

    virtual const QStringList &getErrors() const = 0;
};

typedef QSharedPointer<ISubtitleBlockValidator> ValidatorPtr;
typedef QSharedPointer<const ISubtitleBlockValidator> ValidatorConstPtr;


class SubtitleBlockValidator: public ISubtitleBlockValidator
{
public:
    SubtitleBlockValidator(const QList< RuleConstPtr > &rules)
        : _rules( rules )
    { }

    SubtitleBlockValidator()
        : SubtitleBlockValidator( { RulePtr( new HasHeaderRule() ),
                                    RulePtr( new HasOneFormatRowRule() ),
                                    RulePtr( new ColumnsCountEqualRule() ) } )
    { }

    bool validate(const SubtitleBlock &block) override
    {
        _errors.clear();

        for( const auto &rule: _rules )
        {
            if( !rule->validate( block ) )
            {
                _errors << rule->getError();
            }
        }

        return _errors.isEmpty();
    }

    void setRules(const QList<RuleConstPtr> &rules) override
    {
        _rules = rules;
    }

    void addRules(const QList<RuleConstPtr> &rules) override
    {
        _rules << rules;
    }

    const QStringList &getErrors() const override
    {
        return _errors;
    }


private:
    QList< RuleConstPtr > _rules;
    QStringList _errors;
};


#endif // SUBTITLE_BLOCK_VALIDATOR_H
