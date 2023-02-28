#ifndef SUBTITLE_BLOCK_READER_H
#define SUBTITLE_BLOCK_READER_H

#include <QFile>
#include <QList>
#include <QString>
#include <QStringList>
#include <QSharedPointer>

#include "constants.h"
#include "string_helper.h"
#include "subtitle_block.h"
#include "editor/subtitle_block_validator.h"
#include "subtitle_row.h"
#include "subtitle_row_serializer.h"
#include "subtitles_fixer_exceptions.h"


class ISubtitleBlockReader
{
public:
    virtual ~ISubtitleBlockReader() = default;
    virtual SubtitleBlock readBlock( const QStringList &lines ) = 0;
};

typedef QSharedPointer<ISubtitleBlockReader> SubtitleBlockReaderPtr;

class SubtitleBlockReader: public ISubtitleBlockReader
{
public:
    SubtitleBlockReader( ValidatorPtr validator )
        : _validator( validator )
    { }

    SubtitleBlockReader()
        : SubtitleBlockReader( ValidatorPtr( new SubtitleBlockValidator() ) )
    { }

    SubtitleBlock readBlock(const QStringList &lines) override
    {
        SubtitleBlock subBlock;

        for(const QString &line: lines)
        {
            SubtitleRowPtr subLine = SubtitleRowSerializer::parse( line );

            if( subLine->getDataType() == SubtitleRow::Header )
            {
                const QString &title = subLine->getTitle();
                subBlock.name = title;
                subBlock.blockType = getBlockTypeFromTitle( title );
            }

            subBlock.lines.append( subLine );
        }

        if( !_validator->validate( subBlock ) )
        {
            throw BlockValidationError( _validator->getErrors().join( ", " ) );
        }

        return subBlock;
    }

protected:
    ValidatorPtr _validator;


private:
    inline SubtitleBlock::BlockType getBlockTypeFromTitle( const QString &title )
    {
        if( stringsEqualCaseInsensitive( title, STYLE_BLOCK_TITLE ) )
        {
            return  SubtitleBlock::STYLE_BLOCK;
        }
        else if( stringsEqualCaseInsensitive( title, EVENTS_BLOCK_TITLE ) )
        {
            return SubtitleBlock::EVENTS_BLOCK;
        };

        return SubtitleBlock::OTHER_BLOCK;
    }
};


#endif // SUBTITLE_BLOCK_READER_H
