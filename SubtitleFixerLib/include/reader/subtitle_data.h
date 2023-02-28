#ifndef SUBTITLE_DATA_H
#define SUBTITLE_DATA_H

#include <QList>
#include <QTextStream>

#include "subtitle_block.h"
#include "subtitles_fixer_exceptions.h"
#include "constants.h"


class SubtitleData
{
public:
    SubtitleBlock &getBlock( const QString &blockName )
    {
        for( auto it = blocks.begin(); it != blocks.end(); ++it )
        {
            if( it->name == blockName )
            {
                return *it;
            }
        }

        throw SubtitleBlockNotFoundException( blockName.toStdString() );
    }

    SubtitleBlock &getStylesBlock()
    {
        return getBlock( STYLE_BLOCK_TITLE );
    }

    QList< SubtitleBlock > blocks;

    bool operator==( const SubtitleData &another ) const
    {
        if( this->blocks.length() != another.blocks.length() )
        {
            return false;
        }

        for( qsizetype i = 0; i < this->blocks.length(); ++i )
        {
            if( this->blocks[ i ] != another.blocks[ i ] )
            {
                return false;
            }
        }

        return true;
    }

    QString toString() const
    {
        QString result;
        QTextStream out( &result );
        for( const SubtitleBlock &block: blocks )
        {
            out << block.toString();
        }

        return result;
    }
};

#endif // SUBTITLE_DATA_H
