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
            if( line->getDataType() == SubtitleRow::Formatter )
            {
                return true;
            }
        }

        return false;
    }

    SubtitleRowPtr getFormatLine()
    {
        for( auto it = lines.begin(); it != lines.end(); ++it )
        {
            if( (*it)->getDataType() == SubtitleRow::Formatter )
            {
                return *it;
            }
        }

        throw FormatLineNotFoundException();
    }

    bool operator==( const SubtitleBlock &another ) const
    {
        if( this->name != another.name )
        {
            return false;
        }

        if( this->lines.length() != another.lines.length() )
        {
            return false;
        }

        for( qsizetype i = 0; i < this->lines.length(); ++i )
        {
            if( *(this->lines[ i ]) != *(another.lines[ i ]) )
            {
                return false;
            }
        }

        return true;
    }

    bool operator!=( const SubtitleBlock &another ) const
    {
        return !( (*this) == another );
    }

    QString name;
    QList< SubtitleRowPtr > lines;
};


#endif // SUBTITLE_BLOCK_H
