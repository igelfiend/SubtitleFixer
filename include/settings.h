#ifndef SETTINGS_H

#include <QString>
#include "option.h"
#include "codec_settings.h"


typedef Option< int     > IntOption;
typedef Option< QString > QStringOption;
typedef Option< CodecSettings > CodecSettingsOption;


class Settings
{
public:
    Settings(){}

    void setIncreaseFontSize(int value)
    {
        _increaseFontSize = value;
    }

    void setNewFontName( const QString &newFontname )
    {
        _newFontName = newFontname;
    }

    void setCodecSettings( const CodecSettings &codecSettings )
    {
        _codecSettings = codecSettings;
    }

    IntOption getIncreaseFontSize() const { return _increaseFontSize; }

    QStringOption getNewFontName() const { return _newFontName; }

    CodecSettingsOption getCodecSettings() const { return _codecSettings; }

private:
    IntOption     _increaseFontSize;
    QStringOption _newFontName;
    CodecSettingsOption _codecSettings;
};

#define SETTINGS_H

#endif // SETTINGS_H
