#ifndef SUBTITLE_DATA_H
#define SUBTITLE_DATA_H

#include <QList>
#include "subtitle_block.h"
#include "subtitles_fixer_exceptions.h"

#define STYLES_TITLE "V4+ Styles"
#define EVENTS_TITLE "Events"


class SubtitleData
{
public:
    SubtitleData() { }
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
        return getBlock( STYLES_TITLE );
    }

    SubtitleBlock &getEventsBlock()
    {
        return getBlock( EVENTS_TITLE );
    }

    QList< SubtitleBlock > blocks;
};

#endif // SUBTITLE_DATA_H
