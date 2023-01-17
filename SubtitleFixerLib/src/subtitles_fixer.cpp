#include "subtitles_fixer.h"

#include <QFile>
#include <QTextCodec>
#include <QTextStream>

#include "reader/subtitle_data_serializer.h"
#include "reader/subtitle_data.h"
#include "editor/subtitle_style_editor.h"

SubtitlesFixer::SubtitlesFixer()
{ }

void SubtitlesFixer::setSettings(const Settings &settings)
{
    _settings = settings;
}

bool SubtitlesFixer::fixFile(const QString &filepath, const QString &savepath)
{
    _error = QString();

    try
    {
        QFile inputFile( filepath );
        SubtitleData subData = SubtitleDataSerializer::readFromFile( inputFile );

        SubtitleBlock &styles = subData.getStylesBlock();
        SubtitleRowPtr styleFormat = styles.getFormatLine();

        SubtitleStyleEditor styleEditor( styles );

        auto increaseFont = _settings.getIncreaseFontSize();
        auto newFontName = _settings.getNewFontName();

        if( increaseFont.enabled )
        {
            styleEditor.increaseFontSize( increaseFont.value );
        }

        if( newFontName.enabled )
        {
            styleEditor.changeFontName( newFontName.value );
        }

        QFile outputFile( savepath );
        SubtitleDataSerializer::saveToFile( outputFile,
                                            subData,
                                            _settings.getCodecSettings().value.codecName,
                                            _settings.getCodecSettings().value.hasBom );
    }
    catch( const std::runtime_error &e )
    {
        _error = e.what();
        return false;
    }

    return true;
}

QString SubtitlesFixer::getError() const
{
    return _error;
}
