#ifndef SUBTITLE_BLOCK_VALIDATOR_H
#define SUBTITLE_BLOCK_VALIDATOR_H

#include <QList>
#include <QStringList>
#include <QSharedPointer>

#include "reader/subtitle_block.h"
#include "subtitle_style_rule.h"

typedef QSharedPointer<AbstractSubtitleBlockValidationRule> RulePtr;

class AbstractSubtitleValidator
{
public:
    virtual ~AbstractSubtitleValidator() { }
    virtual bool validate();

    virtual QStringList getErrors() const;
};

class SubtitleBlockValidator: public AbstractSubtitleValidator
{
public:
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


private:
    QList< RulePtr > _rules;
    QStringList _errors;
};


#endif // SUBTITLE_BLOCK_VALIDATOR_H
