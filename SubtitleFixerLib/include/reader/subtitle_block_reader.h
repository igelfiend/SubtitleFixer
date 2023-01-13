#ifndef SUBTITLE_BLOCK_READER_H
#define SUBTITLE_BLOCK_READER_H

#include <QFile>
#include <QList>
#include <QString>
#include <QStringList>

#include "subtitle_block.h"
#include "subtitle_data.h"
#include "subtitle_row.h"
#include "subtitle_row_serializer.h"
#include "subtitles_fixer_exceptions.h"
#include "string_helper.h"


class SubtitleBlockReader
{
public:
    static SubtitleData readFromFile(const QString &filepath)
    {
        QFile file( filepath );
        SubtitleData subData;

        if( !file.open( QIODevice::ReadOnly | QFile::Text ) )
        {
            qCritical() << "SubtitleBlock::readFromFile: Failed to open file for read";
            throw FileNotOpenedException();
        }

        QStringList blockData;
        bool headerRead = false;

        while( !file.atEnd() )
        {
            QString line = file.readLine().trimmed();

            if( headerRead && stringIsBlockHeader( line ) )
            {
                // process lines of data into block
                // push new header into new lines
                subData.blocks.append( createBlock( blockData ) );

                blockData.clear();
                blockData.append( line );
            }
            else if( !headerRead && stringIsBlockHeader( line ) )
            {
                // will be entered once - on the first block
                headerRead = true;
            }
        }
        subData.blocks.append( createBlock( blockData ) );

        file.close();

        return subData;
    }

private:
    static SubtitleBlock createBlock(QStringList lines)
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
            qCritical() << "SubtitleBlockReader::createBlock: created block without header"
                        << ". Processed lines: " << lines;
        }

        return subBlock;
    }
};


#endif // SUBTITLE_BLOCK_READER_H
