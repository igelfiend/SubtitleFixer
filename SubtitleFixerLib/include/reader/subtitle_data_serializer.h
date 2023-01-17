#ifndef SUBTITLE_DATA_SERIALIZER_H
#define SUBTITLE_DATA_SERIALIZER_H

#include <QFile>
#include <QString>
#include <QStringList>
#include <QTextCodec>

#include "subtitle_data.h"
#include "subtitles_fixer_exceptions.h"
#include "string_helper.h"
#include "reader/subtitle_block_reader.h"

class SubtitleDataSerializer
{
public:
    static SubtitleData readFromFile(QFile &file)
    {
        SubtitleData subData;

        if( !file.open( QIODevice::ReadOnly | QFile::Text ) )
        {
            throw FileNotOpenedException( file.fileName() );
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
                subData.blocks.append( SubtitleBlockReader::readBlock( blockData ) );

                blockData.clear();
                blockData.append( line );
            }
            else if( !headerRead && stringIsBlockHeader( line ) )
            {
                // will be entered once - on the first block
                headerRead = true;
                blockData.append( line );
            }
            else
            {
                blockData.append( line );
            }
        }
        subData.blocks.append( SubtitleBlockReader::readBlock( blockData ) );

        file.close();

        return subData;
    }

    static void saveToFile(QFile &file, const SubtitleData &data, const QString &codecName, bool codecHasBom)
    {
        if( !file.open( QIODevice::WriteOnly | QFile::Text ) )
        {
            throw FileNotOpenedException( file.fileName() );
        }

        QTextCodec *codec = QTextCodec::codecForName( codecName.toUtf8() );
        QTextDecoder *decoder = codec->makeDecoder();

        QTextStream out( &file );
        out.setGenerateByteOrderMark( codecHasBom );

        out << decoder->toUnicode( data.toString().toUtf8() );

        file.close();
    }
};


#endif // SUBTITLE_DATA_SERIALIZER_H
