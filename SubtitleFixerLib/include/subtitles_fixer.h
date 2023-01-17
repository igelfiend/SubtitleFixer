#ifndef SUBTITLES_FIXER_H
#define SUBTITLES_FIXER_H

#include <QString>

#include "settings.h"


/**
 * @brief Used for fixing subtitle file.
 * @details all fix parameters can be set via settings.
 * Don't use the same path for read/save because could be owner rights issue
 */
class SubtitlesFixer
{
public:
    explicit SubtitlesFixer();

    void setSettings( const Settings &settings );

    /**
     * @brief Updates subtitle file on `filepath` and saves it on `savepath`.
     * @param filepath - path for subtitle file.
     * @param filepath - path for saving result file.
     * @returns `true` on success, `false` if something goes wrong. Error can be get via `getError`
     */
    bool fixFile( const QString &filepath, const QString &savepath);

    QString getError() const;

private:
    Settings _settings;
    QString _error;
};

#endif // SUBTITLES_FIXER_H
