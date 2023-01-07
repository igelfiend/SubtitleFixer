#include "reader/subtitle_row_serializer.h"

#include <stdexcept>
#include <QStringView>
#include <QDebug>

#include "string_helper.h"

SubtitleRowSerializer::SubtitleRowSerializer()
{

}

SubtitleRow SubtitleRowSerializer::parse(const QString &row)
{
    QString rowTrimmed = row.trimmed();
    if( rowTrimmed.isEmpty() )
    {
        return EmptySubtitleRow();
    }

    if( rowTrimmed.startsWith(';') )
    {
        return CommentSubtitleRow( rowTrimmed );
    }

    if( stringIsBlockHeader(rowTrimmed) )
    {
        return HeaderSubtitleRow(rowTrimmed);
    }

    if( rowTrimmed.contains(':') )
    {
        if( rowTrimmed.startsWith("Format:", Qt::CaseInsensitive) )
        {
            return FormatterSubtitleRow(rowTrimmed);
        }
        else
        {
            return ContentSubtitleRow(rowTrimmed);
        }
    }

    qWarning() << "SubtitleRowSerializer: row type not recognized. Row: \"" << row << "\"";

    return UnknownSubtitleRow(rowTrimmed);
}
