#ifndef SUBTITLE_STYLE_RULE_H
#define SUBTITLE_STYLE_RULE_H

#include <QString>

#include "subtitle_style_validator.h"
#include "reader/subtitle_row.h"


class AbstractSubtitleBlockValidationRule
{
public:
    virtual ~AbstractSubtitleBlockValidationRule() { }

    virtual bool validate() const;
    virtual QString getError() const;
};



class BaseSubtitleBlockValidationRule: public AbstractSubtitleBlockValidationRule
{
public:
    BaseSubtitleBlockValidationRule(const SubtitleStyleValidator &validator, const QString &error)
        : _validator(validator)
        , _error(error)
    { }

    bool validate() const override;
    QString getError() const override
    {
        return _error;
    }

protected:
    const SubtitleStyleValidator &_validator;

private:
    const QString _error;
};


class HasHeaderRule: BaseSubtitleBlockValidationRule
{
public:
    HasHeaderRule(const SubtitleStyleValidator &validator)
        : BaseSubtitleBlockValidationRule(validator, "Header not found or invalid in block")
    { }

    bool validate() const override
    {
        const SubtitleBlock &block = this->_validator.getBlockForValidation();
        return  ( block.lines.count() > 0 )
                && ( block.lines.at(0).dataType == SubtitleRow::Header )
                && ( !block.lines.at(0).title.isEmpty() );
    }
};


class HasFormatRowRule: BaseSubtitleBlockValidationRule
{
public:
    HasFormatRowRule(const SubtitleStyleValidator &validator)
        : BaseSubtitleBlockValidationRule(validator, "Format row not found in block")
    { }

    bool validate() const override
    {
        const SubtitleBlock &block = this->_validator.getBlockForValidation();
        return block.hasFormatLine();
    }
};


class HasFormatRowRule: BaseSubtitleBlockValidationRule
{
public:
    HasFormatRowRule(const SubtitleStyleValidator &validator)
        : BaseSubtitleBlockValidationRule(validator, "Format row not found in block")
    { }

    bool validate() const override
    {
        const SubtitleBlock &block = this->_validator.getBlockForValidation();
        return block.hasFormatLine();
    }
};



#endif // SUBTITLE_STYLE_RULE_H
