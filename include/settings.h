#ifndef SETTINGS_H

#include <QString>
#include "option.h"


typedef Option< int     > IntOption;
typedef Option< QString > QStringOption;

/**
 * @brief The SubtitlesFixData class used for storing data which used for updating subtitle files.
 * Important: By default, any field enable flag is turned off.
 * But, when user calls setter, it's stands "true" (From FixField constructor).
 */
class Settings
{
public:
    Settings(){}

    /**
     * @brief Sets increase value for font size. When called, that field stands enabled.
     * @param value - increase value.
     */
    void setIncreaseFontSize(int value)
    {
        _increaseFontSize = value;
    }

    /**
     * @brief Sets new bont name value. When called, that field stands enabled.
     * @param newFontname - new font name.
     */
    void setNewFontName( const QString &newFontname )
    {
        _newFontName = newFontname;
    }

    /**
     * @brief Font size increase getter.
     * @return font size increase field.
     */
    IntOption getIncreaseFontSize() const { return _increaseFontSize; }

    /**
     * @brief Font name getter.
     * @return font name field,
     */
    QStringOption getNewFontName() const { return _newFontName; }

private:
    IntOption     _increaseFontSize;  ///< font size increase field.
    QStringOption _newFontName;       ///< font name field.
};

#define SETTINGS_H

#endif // SETTINGS_H
