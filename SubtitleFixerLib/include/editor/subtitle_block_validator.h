#ifndef SUBTITLE_BLOCK_VALIDATOR_H
#define SUBTITLE_BLOCK_VALIDATOR_H

#include <QList>
#include <QStringList>
#include <QSharedPointer>

#include "reader/subtitle_block.h"
#include "subtitle_block_rule.h"

typedef QSharedPointer<AbstractSubtitleBlockValidationRule> RulePtr;

class AbstractSubtitleValidator
{
public:
    virtual ~AbstractSubtitleValidator() { }
    virtual bool validate() = 0;
    virtual void setRules(const QList<RulePtr>&) = 0;
    virtual void addRules(const QList<RulePtr>&) = 0;

    virtual const QStringList &getErrors() const = 0;
};

class SubtitleBlockValidator: public AbstractSubtitleValidator
{
public:
    SubtitleBlockValidator()
    { }

    SubtitleBlockValidator(const SubtitleBlock &block)
        :_rules{ RulePtr( new HasHeaderRule        ( block ) ),
                 RulePtr( new HasFormatRowRule     ( block ) ),
                 RulePtr( new ColumnsCountEqualRule( block ) ) }
    { }

    bool validate() override
    {
        _errors.clear();

        for( const auto &rule: _rules )
        {
            if( !rule->validate() )
            {
                _errors << rule->getError();
            }
        }

        return _errors.isEmpty();
    }

    void setRules(const QList<RulePtr> &rules) override
    {
        _rules = rules;
    }

    void addRules(const QList<RulePtr> &rules) override
    {
        _rules << rules;
    }

    const QStringList &getErrors() const override
    {
        return _errors;
    }


private:
    QList< RulePtr > _rules;
    QStringList _errors;
};


#endif // SUBTITLE_BLOCK_VALIDATOR_H
