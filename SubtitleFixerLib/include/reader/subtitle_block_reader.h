#ifndef SUBTITLE_BLOCK_READER_H
#define SUBTITLE_BLOCK_READER_H

#include <QFile>
#include <QList>
#include <QString>
#include <QStringList>

#include "subtitle_block.h"
#include "subtitle_row.h"
#include "subtitle_row_serializer.h"
#include "subtitles_fixer_exceptions.h"


class SubtitleBlockReader
{
public:
    static SubtitleBlock readBlock(QStringList lines)
    {
        SubtitleBlock subBlock;
        bool headerRead = false;


        for(const QString &line: lines)
        {
            SubtitleRowPtr subLine = SubtitleRowSerializer::parse( line );

            if( subLine->getDataType() == SubtitleRow::Header )
            {
                subBlock.name = subLine->getTitle();
                headerRead = true;
            }

            subBlock.lines.append( subLine );
        }

        if( !headerRead )
        {
            qCritical() << "SubtitleBlockReader::readBlock: created block without header"
                        << ". Processed lines: " << lines;
            throw BlockHeaderNotFoundException();
        }

        return subBlock;
    }
};


#endif // SUBTITLE_BLOCK_READER_H
