#include "subtitles_fixer.h"

#include <QDebug>
#include <QFile>
#include <QRegularExpression>
#include <QTextCodec>
#include <QTextStream>

#include "subtitles_fixer_exceptions.h"

SubtitlesFixer::SubtitlesFixer(QObject *parent)
    : QObject(parent)
{ }

void SubtitlesFixer::setSettings(const Settings &settings)
{
    _settings = settings;
}

bool SubtitlesFixer::fixFile(const QString &filepath, const QString &savepath)
{
    //! Clear previous data
    _header.clear();
    _fileData.clear();

    try
    {
        //! Reading subtitle file
        readFile( filepath );

        //! Receiving start of the style block
        int currentRow = getStartOfTheStyleGroupIndex();

        //! Reading style header and moving start ptr on the next row
        readHeader( currentRow );
        ++currentRow;

        processStyleData( currentRow );

        //! Saving result file
        //! (overwriting existing file was failed in tests, so it's recomended to save in new file)
        saveFile( savepath );
    }
    catch( const std::runtime_error &e )
    {
        QTextStream error;
        error << "SubtitlesFixer: fix for file " << filepath
              << " was failed with " << e.what() << " exception.";
        emit signalError(*error.string());

        return false;
    }

    return true;
}

void SubtitlesFixer::readFile(const QString &filepath)
{
    //! Receiving data
    QFile file( filepath );

    if( !file.exists() )
    {
        const QString error = QString("File not found!\nFile: %1").arg(filepath);
        emit signalError(error);
        qCritical() << error;

        throw FileNotFoundException();
    }

    if( !file.open( QIODevice::ReadOnly | QFile::Text ) )
    {
        const QString error = QString("Failed while openning file in read mode!\nFile: %1").arg(filepath);
        emit signalError(error);
        qCritical() << error;

        throw OpenFileErrorException();
    }

    while( !file.atEnd() )
    {
        _fileData.append( file.readLine() );
    }

    file.close();
}

void SubtitlesFixer::saveFile(const QString &filepath)
{
    QFile outFile( filepath );
    if( !outFile.open( QIODevice::WriteOnly | QFile::Text ) )
    {
        QTextStream errorStream;
        errorStream << "Failed while openning file in write mode.\nFile: "
                    << filepath
                    << "\n error: "
                    << outFile.error();
        emit signalError(*errorStream.string());
        qCritical() << errorStream.string();

        throw OpenFileErrorException();
    }

    const std::string codecName = _settings.getCodecSettings().value.codecName.toStdString();
    bool codecHasBom = _settings.getCodecSettings().value.hasBom;
    QTextCodec *codec = QTextCodec::codecForName( codecName.c_str() );
    QTextDecoder *decoder = codec->makeDecoder();
    QTextStream out( &outFile );
    out.setGenerateByteOrderMark( codecHasBom );
    foreach( const QString &row, _fileData )
    {
        out << decoder->toUnicode(row.toStdString().c_str());
    }

    outFile.close();
}

int SubtitlesFixer::getStartOfTheStyleGroupIndex() const
{
    QString fontSettingsGroupName( "[V4+ Styles]" );
    for( int i = 0; i < _fileData.length(); ++i )
    {
        //! Searching group name substring
        //! and also checking that's it not in the end of file
        if(  ( _fileData[ i ].trimmed().compare( fontSettingsGroupName, Qt::CaseInsensitive ) == 0 )
          && ( (i+1) < _fileData.length() ) )
        {
            return ( i+1 );
        }
    }

    qCritical() << "Style position detection failed. Printing all processed data and compare operations";
    for( int i = 0; i < _fileData.length(); ++i )
    {
        //! Searching group name substring
        //! and also checking that's it not in the end of file
        qCritical() << "failed row: " << _fileData[ i ].trimmed();
    }

    //! If we pass through all file lines and not found any signs of style block, throws exception
    throw SettingsGroupNotFoundException();
}

void SubtitlesFixer::readHeader(int headerIndex)
{
    //! Making copy for header processing
    QString headerRow = _fileData[ headerIndex ];

    //! Removing "Format: " at the beginning of the string
    headerRow.remove( 0, 8 );

    //! Removing all whitespacing
    headerRow.remove( QRegularExpression( "\\s" ) );

    //! Receiving list of header elements
    _header = headerRow.split( "," );
}

void SubtitlesFixer::processRow(int rowIndex)
{
    //! If we receive empty string, just skip it
    if( _fileData[ rowIndex ].trimmed().isEmpty() )
    {
        return;
    }

    //! Copying source row
    QString rowStr( _fileData[ rowIndex ] );

    //! Cutting start prefix "Style: " from row
    QString rowStart = rowStr.left( 7 );
    rowStr.remove( 0, 7 );

    //! Splitting all data into components, which was separated with ","
    QStringList rowList = rowStr.split( "," );
    try
    {
        //! Updating fontname and fontsize
        //! If anything goes wrong, execpting will throw at upper level
        //! If any replacement not required, it will be skipped inside update function
        //! * Fontsize will be increased/decreased
        //! * Fontname will be replaced
        updateFontname( rowList );
        updateFontsize( rowList );
    }
    catch( ... )
    {
        throw;
    }

    //! Recovering row and saving into file data
    _fileData[ rowIndex ] = rowStart + rowList.join( "," );
}

void SubtitlesFixer::updateFontname(QStringList &row)
{
    if( !_settings.getNewFontName().enabled )
    {
        return;
    }

    int columnIndex = _header.indexOf( "Fontname" );
    if( columnIndex < 0 )
    {
        throw ColumnNotFoundException( "Fontname column in header not found" );
    }

    row[ columnIndex ] = _settings.getNewFontName().value;
}

bool SubtitlesFixer::isBlockDeclaration(QString str)
{
    str = str.trimmed();
    return   (str.isEmpty() == false)
          && ( str.front() == '[' )
          && ( str.back()  == ']' );
}

void SubtitlesFixer::processStyleData(int index)
{
    //! Processing all data block until we hit new block
    //! (in additional providing checking end of data)
    try
    {
        while(  ( index < _fileData.length() )
             && ( !isBlockDeclaration( _fileData[ index ] ) ))
        {
            processRow( index );
            ++index;
        }
    }
    catch(...)
    {
        throw;
    }
}

void SubtitlesFixer::updateFontsize(QStringList &row)
{
    if( !_settings.getIncreaseFontSize().enabled )
    {
        return;
    }

    int columnIndex = _header.indexOf( "Fontsize" );
    if( columnIndex < 0 )
    {
        throw ColumnNotFoundException( "Fontsizes column in header not found" );
    }

    //! Extracting font data and trying to convert it into int
    QString fontData = row[ columnIndex ];

    bool fSuccess = true;
    int newFont = fontData.toInt( &fSuccess );

    if( !fSuccess )
    {
        qCritical() << "Convertion do Int failed";
        qCritical() << fontData << " -> " << newFont;
        throw ConvertionToIntFailedException();
    }

    //! Updating with new font size
    row[ columnIndex ] = QString::number( newFont + _settings.getIncreaseFontSize().value );
}
