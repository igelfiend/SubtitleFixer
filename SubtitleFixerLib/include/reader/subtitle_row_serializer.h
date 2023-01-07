#ifndef SUBTITLEROWSERIALIZER_H
#define SUBTITLEROWSERIALIZER_H

#include <QString>
#include "reader/subtitle_row.h"


class SubtitleRowSerializer
{
public:
    SubtitleRowSerializer();

    static SubtitleRow parse(const QString &row);
};

#endif // SUBTITLEROWSERIALIZER_H