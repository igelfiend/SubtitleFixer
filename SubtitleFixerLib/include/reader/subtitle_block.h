#ifndef SUBTITLE_BLOCK_H
#define SUBTITLE_BLOCK_H

#include <algorithm>
#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QString>
#include <QList>
#include <QTextStream>

#include "subtitle_row.h"
#include "subtitles_fixer_exceptions.h"


class SubtitleBlock
{
public:
    enum BlockType: int
    {
        STYLE_BLOCK  = 0,
        EVENTS_BLOCK = 1,
        OTHER_BLOCK = 2,
    };

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

    QList< SubtitleRowPtr > getDataLines() const
    {
        QList< SubtitleRowPtr > dataLines;
        auto func_only_content = []( const SubtitleRowPtr &line ){ return line->getDataType() == SubtitleRow::Content; };
        std::copy_if( lines.begin(),
                      lines.end(),
                      std::back_inserter( dataLines ),
                      func_only_content );
        return dataLines;
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

    QString toString() const
    {
        QString result;
        QTextStream out( &result );
        for( auto const &line: lines )
        {
            out << line->toString() << "\n";
        }

        return result;
    }

    QString name;
    QList< SubtitleRowPtr > lines;
    BlockType blockType = OTHER_BLOCK;
};


#endif // SUBTITLE_BLOCK_H
