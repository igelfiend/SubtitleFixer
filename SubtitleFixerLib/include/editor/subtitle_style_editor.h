#ifndef SUBTITLE_STYLE_EDITOR_H
#define SUBTITLE_STYLE_EDITOR_H

#include <functional>

#include "editor/subtitle_block_editor.h"
#include "reader/subtitle_block.h"
#include "subtitles_fixer_exceptions.h"
#include "constants.h"

class SubtitleStyleEditor
{
public:
    SubtitleStyleEditor(SubtitleBlock &styleBlock)
        : _styleBlock( styleBlock )
        , _editor( styleBlock )
    {
        if( styleBlock.name.compare( STYLE_BLOCK_TITLE, Qt::CaseInsensitive ) != 0 )
        {
            throw ExpectedStyleBlockException( styleBlock.name );
        }
    }



    void changeFontName( const QString &newFontName )
    {
        _editor.updateAllValuesForField(
            "Fontname",
            [ &newFontName ]( const QString &element ){ return newFontName; }
        );
    }

    void increaseFontSize( int increaseValue )
    {
        _editor.updateAllValuesForField(
            "Fontsize",
            [ increaseValue ]( const QString &element )
            {
                bool fSuccess = true;
                int value = element.toInt( &fSuccess );
                if( !fSuccess )
                {
                    throw ConvertionToIntFailedException( element );
                }

                value += increaseValue;

                return QString::number( value );
            }
        );
    }

private:
    SubtitleBlock _styleBlock;
    SubtitleBlockEditor _editor;
};


#endif // SUBTITLE_STYLE_EDITOR_H
