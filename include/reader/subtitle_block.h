#ifndef SUBTITLE_BLOCK_H
#define SUBTITLE_BLOCK_H

#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QString>
#include <QList>

#include "subtitle_row.h"
#include "subtitles_fixer_exceptions.h"


class SubtitleBlock
{
public:
    bool hasFormatLine() const
    {
        for( const auto &line: lines )
        {
            if( line.dataType == SubtitleRow::Formatter )
            {
                return true;
            }
        }

        return false;
    }

    SubtitleRow &getFormatLine()
    {
        for( auto it = lines.begin(); it != lines.end(); ++it )
        {
            if( it->dataType == SubtitleRow::Formatter )
            {
                return *it;
            }
        }

        throw FormatLineNotFoundException();
    }


    QString name;
    QList< SubtitleRow > lines;
};


#endif // SUBTITLE_BLOCK_H
