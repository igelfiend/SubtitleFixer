#include "subtitles_fixer.h"

#include <QDebug>
#include <QRegExp>

#include "subtitle_fixer_exceptions.h"

SubtitlesFixer::SubtitlesFixer()
    : _codec( "UTF-8" )
    , _hasBom( false )
{ }

void SubtitlesFixer::setCodec(const QString &codecName, bool bom)
{
    _codec = codecName;
    _hasBom = bom;
}

bool SubtitlesFixer::fixFile(const QString &filepath, const QString &savepath, const SubtitlesFixData &fixData)
{
    _fixData = fixData;

    try
    {
        //! Reading subtitle file
        readFile( filepath );

        //! Receiving start of the style block
        int currentRow = getStartOfTheStyleGroupIndex();

        //! Reading style header and moving start ptr on the next row
        readHeader( currentRow );
        ++currentRow;

        //! Processing all data block until we hit empty row
        //! (in additional providing checking end of data)
        while(  ( currentRow < _fileData.length() )
             && !_fileData[ currentRow ].trimmed().isEmpty() )
        {
            processRow( currentRow );
            ++currentRow;
        }

        //! Saving result file
        //! (overwriting existing file was failed in tests, so it's recomended to save in new file)
        saveFile( savepath );
    }
    catch( const std::runtime_error &e )
    {
        qCritical() << "SubtitlesFixer: fix for file " << filepath
                    << " was failed with " << e.what() << " exception.";
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
        qCritical() << "File not found!\nFile: " << filepath;
        throw NoSuchFileException();
    }

    if( !file.open( QIODevice::ReadOnly | QFile::Text ) )
    {
        qCritical() << "Failed while openning file in read mode!\nFile: " << filepath;
        throw OpenningFileErrorException();
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
        qCritical() << "Failed while openning file in write mode.\nFile: " << filepath;
        qCritical() << "error: " << outFile.error();
        throw OpenningFileErrorException();
    }

    QTextStream out( &outFile );
    out.setCodec( _codec.toStdString().c_str() );
    out.setGenerateByteOrderMark( _hasBom );
    foreach( const QString &row, _fileData )
    {
        out << row;
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
    headerRow.remove( QRegExp( "\\s" ) );

    //! Receiving list of header elements
    _header = headerRow.split( "," );
}

void SubtitlesFixer::processRow(int rowIndex)
{
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
    if( !_fixData.getNewFontName().enabled )
    {
        return;
    }

    int columnIndex = _header.indexOf( "Fontname" );
    if( columnIndex < 0 )
    {
        throw ColumnNotFoundException( "Fontname column in header not found" );
    }

    row[ columnIndex ] = _fixData.getNewFontName().value;
}

void SubtitlesFixer::updateFontsize(QStringList &row)
{
    if( !_fixData.getIncreaseFontSize().enabled )
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
    row[ columnIndex ] = QString::number( newFont + _fixData.getIncreaseFontSize().value );
}
