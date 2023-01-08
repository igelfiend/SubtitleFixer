#include "reader/subtitle_row_serializer.h"

#include <stdexcept>
#include <QStringView>
#include <QDebug>

#include "string_helper.h"

SubtitleRowSerializer::SubtitleRowSerializer()
{

}

SubtitleRowPtr SubtitleRowSerializer::parse(const QString &row)
{
    QString rowTrimmed = row.trimmed();
    if( rowTrimmed.isEmpty() )
    {
        return SubtitleRowPtr( new EmptySubtitleRow() );
    }

    if( rowTrimmed.startsWith(';') )
    {
        return SubtitleRowPtr( new CommentSubtitleRow( rowTrimmed ) );
    }

    if( stringIsBlockHeader(rowTrimmed) )
    {
        return SubtitleRowPtr( new HeaderSubtitleRow( rowTrimmed ) );
    }

    if( rowTrimmed.contains(':') )
    {
        if( rowTrimmed.startsWith("Format:", Qt::CaseInsensitive) )
        {
            return SubtitleRowPtr( new FormatterSubtitleRow( rowTrimmed ) );
        }
        else
        {
            return SubtitleRowPtr( new ContentSubtitleRow( rowTrimmed ) );
        }
    }

    qWarning() << "SubtitleRowSerializer: row type not recognized. Row: \"" << row << "\"";

    return SubtitleRowPtr( new UnknownSubtitleRow( rowTrimmed ) );
}
