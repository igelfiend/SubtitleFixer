#ifndef SUBTITLE_STYLE_RULE_H
#define SUBTITLE_STYLE_RULE_H

#include <QString>

#include "reader/subtitle_block.h"
#include "reader/subtitle_row.h"


class AbstractSubtitleBlockValidationRule
{
public:
    virtual ~AbstractSubtitleBlockValidationRule() { }

    virtual bool validate() const = 0;
    virtual QString getError() const = 0;
};

class BaseSubtitleBlockValidationRule: public AbstractSubtitleBlockValidationRule
{
public:
    BaseSubtitleBlockValidationRule(const SubtitleBlock &block, const QString &error)
        : _block(block)
        , _error(error)
    { }
    ~BaseSubtitleBlockValidationRule() { }

    QString getError() const override
    {
        return _error;
    }

protected:
    const SubtitleBlock &_block;

private:
    const QString _error;
};


class HasHeaderRule: public BaseSubtitleBlockValidationRule
{
public:
    HasHeaderRule(const SubtitleBlock &block)
        : BaseSubtitleBlockValidationRule(block, "Header not found or invalid in block")
    { }
    ~HasHeaderRule() { }

    bool validate() const override
    {
        return  ( _block.lines.count() > 0 )
                && ( _block.lines.at(0)->getDataType() == SubtitleRow::Header )
                && ( !_block.lines.at(0)->getTitle().isEmpty() );
    }
};


class HasFormatRowRule: public BaseSubtitleBlockValidationRule
{
public:
    HasFormatRowRule(const SubtitleBlock &block)
        : BaseSubtitleBlockValidationRule(block, "Format row not found in block")
    { }
    ~HasFormatRowRule() { }

    bool validate() const override
    {
        return _block.hasFormatLine();
    }
};


class ColumnsCountEqualRule: public BaseSubtitleBlockValidationRule
{
public:
    ColumnsCountEqualRule(const SubtitleBlock &block)
        : BaseSubtitleBlockValidationRule(block, "No data or count of columns is not equal")
    { }
    ~ColumnsCountEqualRule() { }

    bool validate() const override
    {
        QList< SubtitleRowPtr > contentfulLines;
        for( const auto &line: _block.lines )
        {
            if( line->isStoreValues() )
            {
                contentfulLines << line;
            }
        }

        if( contentfulLines.empty() )
        {
            return true;
        }

        auto rowIt = contentfulLines.begin();
        int columnsCount = (*rowIt)->getValues().length();
        for(; rowIt != contentfulLines.end(); ++rowIt)
        {
            if( columnsCount != (*rowIt)->getValues().length() )
            {
                return false;
            }
        }

        return true;
    }
};



#endif // SUBTITLE_STYLE_RULE_H
