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

    qsizetype splitterIndex = rowTrimmed.indexOf(':');
    if( splitterIndex >= 0 )
    {
        if( rowTrimmed.startsWith("Format:", Qt::CaseInsensitive) )
        {
            return FormatterSubtitleRow(rowTrimmed, splitterIndex);
        }
        else
        {
            return ContentSubtitleRow(rowTrimmed, splitterIndex);
        }
    }

    qWarning() << "SubtitleRowSerializer: row type not recognized. Row: \"" << row << "\"";

    return UnknownSubtitleRow(rowTrimmed);
}
