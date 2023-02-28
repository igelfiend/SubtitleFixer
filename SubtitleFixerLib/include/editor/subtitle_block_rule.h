#ifndef SUBTITLE_BLOCK_RULE_H
#define SUBTITLE_BLOCK_RULE_H

#include <QString>

#include "reader/subtitle_block.h"
#include "reader/subtitle_row.h"


class AbstractSubtitleBlockValidationRule
{
public:
    virtual ~AbstractSubtitleBlockValidationRule() { }

    virtual bool validate(const SubtitleBlock&) const = 0;
    virtual QString getError() const = 0;
};

typedef QSharedPointer<AbstractSubtitleBlockValidationRule> RulePtr;
typedef QSharedPointer<const AbstractSubtitleBlockValidationRule> RuleConstPtr;

class BaseSubtitleBlockValidationRule: public AbstractSubtitleBlockValidationRule
{
public:
    BaseSubtitleBlockValidationRule(const QString &error)
        : _error(error)
    { }
    ~BaseSubtitleBlockValidationRule() { }

    QString getError() const override
    {
        return _error;
    }

private:
    const QString _error;
};


class HasHeaderRule: public BaseSubtitleBlockValidationRule
{
public:
    HasHeaderRule()
        : BaseSubtitleBlockValidationRule("Header not found or invalid in block")
    { }
    ~HasHeaderRule() { }

    bool validate(const SubtitleBlock &block) const override
    {
        if( block.lines.isEmpty() )
        {
            return false;
        }

        int32_t headers_count = 0;
        SubtitleRowPtr header = nullptr;
        const QList< SubtitleRowPtr > lines = block.lines;
        for( auto it = lines.begin(); it != lines.end(); it++ )
        {
            if( (*it)->getDataType() == SubtitleRow::Header )
            {
                header = (*it);
                headers_count++;
            }
        }

        return ( headers_count == 1 ) && ( !header->getTitle().isEmpty() );
    }
};


class HasOneFormatRowRule: public BaseSubtitleBlockValidationRule
{
public:
    HasOneFormatRowRule()
        : BaseSubtitleBlockValidationRule("Format row not found in block")
    { }
    ~HasOneFormatRowRule() { }

    bool validate(const SubtitleBlock &block) const override
    {
        if( isFormatRowRequired( block ) )
        {
            uint32_t format_rows_count = 0;
            for( const auto &line: block.lines )
            {
                if( line->getDataType() == SubtitleRow::Formatter )
                {
                    format_rows_count++;
                }
            }

            return ( format_rows_count == 1 );
        }

        return true;
    }

private:
    bool isFormatRowRequired(const SubtitleBlock &block) const
    {
        return ( block.blockType == SubtitleBlock::STYLE_BLOCK)
            || ( block.blockType == SubtitleBlock::EVENTS_BLOCK);
    }
};


class ColumnsCountEqualRule: public BaseSubtitleBlockValidationRule
{
public:
    ColumnsCountEqualRule()
        : BaseSubtitleBlockValidationRule("No data or count of columns is not equal")
    { }
    ~ColumnsCountEqualRule() { }

    bool validate(const SubtitleBlock &block) const override
    {
        QList< SubtitleRowPtr > contentfulLines;
        for( const auto &line: block.lines )
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



#endif // SUBTITLE_BLOCK_RULE_H
