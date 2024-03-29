#ifndef SUBTITLES_FIXER_H
#define SUBTITLES_FIXER_H

#include <QString>
#include <QMap>
#include <QObject>
#include <QList>
#include <QPair>

#include "settings.h"


/**
 * @brief The SubtitlesFixer class used for updating subtitle files.
 * At start, can be configured with codec. Default codec is UTF-8 with BOM.
 * For each perform iteration process one subtitle file.
 * Important: Not recommended to rewrite source sub files with new one,
 * In test stage that way was failed with owner right.
 */
class SubtitlesFixer: public QObject
{
    Q_OBJECT
//! Public functions
public:
    explicit SubtitlesFixer(QObject *parent = nullptr);

    void setSettings( const Settings &settings );

    /**
     * @brief Function for updating subtitle file using update data.
     * @param filepath - path for subtitle file.
     * @param filepath - path for saving result file.
     * @exception NoSuchFileException - throws when file by filepath doesn't exists.
     * @exception SettingsGroupNotFoundException - throws when settings group not found.
     */
    bool fixFile( const QString &filepath, const QString &savepath);

signals:
    void signalLog(QString);
    void signalError(QString);

private:
    /**
     * @brief Reads subtitle file.
     * @exception NoSuchFileException - when file doesn't exists.
     * @exception OpenningFileErrorException - when failed opening file for reading.
     * @param filepath - read path.
     */
    void readFile( const QString &filepath );

    /**
     * @brief Saves subtitle file.
     * @exception OpenningFileErrorException - when failed opening file for writing.
     * @param filepath - path for saving.
     */
    void saveFile( const QString &filepath );

    /**
     * @brief Searches where starts styles block.
     * @exception SettingsGroupNotFoundException - when style block wasn't found.
     * @return index of the style block.
     */
    int getStartOfTheStyleGroupIndex() const;

    /**
     * @brief Reads header of the style block.
     * @param headerIndex - index of the start style block.
     */
    void readHeader( int headerIndex );

    /**
     * @brief Process row of the style data. Current process steps:
     * - updates fontsize;
     * - updates fontname.
     * @param rowIndex - index of processed row in file data.
     */
    void processRow( int rowIndex );

    /**
     * @brief Updates fontsize using _fixData.
     * @param row - reference to processed row.
     */
    void updateFontsize( QStringList &row );

    /**
     * @brief Updates fontname using _fixData.
     * @param row - reference to processed row.
     */
    void updateFontname( QStringList &row );

    /**
     * @brief Checks if passed row is a declaration of settings block (like [Example])
     * @param str - checked row
     * @return true if str is settings block declaration
     */
    bool isBlockDeclaration(QString str );

    /**
     * @brief Process all data in style block, replaces fontnames and fontsizes
     * @param index - index of the start data
     */
    void processStyleData( int index );

//! Private members
private:
    QStringList _header;        ///< List of queued style-block header titles
    QStringList _fileData;      ///< All-readed data from file
    Settings _settings;
};

#endif // SUBTITLES_FIXER_H
