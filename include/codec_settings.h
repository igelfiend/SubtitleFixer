#ifndef CODEC_SETTINGS_H

#include <QString>

#define DEFAULT_CODEC_NAME "UTF-8"


class CodecSettings
{
public:
    CodecSettings(const QString &codecName = DEFAULT_CODEC_NAME, bool hasBom = true)
        : codecName(codecName)
        , hasBom(hasBom) { }

    QString codecName;
    bool hasBom;
};

#define CODEC_SETTINGS_H

#endif // CODEC_OPTION_H
