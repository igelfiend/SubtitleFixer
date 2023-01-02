#ifndef SETTINGS_H

#include <QString>
#include "option.h"


typedef Option< int     > IntOption;
typedef Option< QString > QStringOption;


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
    IntOption getIncreaseFontSize() const { return _increaseFontSize; }

    QStringOption getNewFontName() const { return _newFontName; }

private:
    IntOption     _increaseFontSize;
    QStringOption _newFontName;
};

#define SETTINGS_H

#endif // SETTINGS_H
