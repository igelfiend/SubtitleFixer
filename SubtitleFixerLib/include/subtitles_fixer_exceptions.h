#ifndef SUBTITLES_FIXER_EXCEPTIONS_H
#define SUBTITLES_FIXER_EXCEPTIONS_H

#include <stdexcept>
#include <string>
#include <QString>

class FileNotFoundException: public std::runtime_error
{
public:
    FileNotFoundException(): std::runtime_error( "File with such name not found" ) { }
};

class OpenFileErrorException: public std::runtime_error
{
public:
    OpenFileErrorException(): std::runtime_error( "Failed while openning file" ) { }
};

class SettingsGroupNotFoundException: public std::runtime_error
{
public:
    SettingsGroupNotFoundException(): std::runtime_error( "Settings group not found" ) { }
};

class FontsizeNotFoundException: public std::runtime_error
{
public:
    FontsizeNotFoundException(): std::runtime_error( "Fontsize field not found" ) { }
};

class ColumnNotFoundException: public std::runtime_error
{
public:
    ColumnNotFoundException( const std::string &str ): std::runtime_error( str ) { }
};

class ConvertionToIntFailedException: public std::runtime_error
{
public:
    ConvertionToIntFailedException( const QString &value )
        : std::runtime_error(
              QString( "Failed while trying to convert \"%1\" to int" )
              .arg( value )
              .toStdString() )
    { }
};


class FileNotOpenedException: public std::runtime_error
{
public:
    FileNotOpenedException(const QString &filename)
        : std::runtime_error(
              QString( "Unable to open file: %1" )
              .arg( filename )
              .toStdString() )
    { }
};


class SubtitleBlockNotFoundException: public std::runtime_error
{
public:
    SubtitleBlockNotFoundException(const std::string &blockName)
        : std::runtime_error( blockName + " block not found") { }
};


class FormatLineNotFoundException: public std::runtime_error
{
public:
    FormatLineNotFoundException(): std::runtime_error( "Format line in subtitle block not found" ) { }
};


class FormatterRowNotFoundException: public std::runtime_error
{
public:
    FormatterRowNotFoundException(): std::runtime_error( "Formatter block not found" ) { }
};


class ColumnNameNotFromFormatLineException: public std::runtime_error
{
public:
    ColumnNameNotFromFormatLineException( const QString &columnName )
        : std::runtime_error(
              QString( "Trying to process \"%1\" column that not from format line" )
              .arg( columnName )
              .toStdString()
        )
    { }
};


class ExpectedStyleBlockException: public std::runtime_error
{
public:
    ExpectedStyleBlockException( const QString &blockName )
        : std::runtime_error(
              QString( "Expected style block, \"%1\" got" )
              .arg( blockName )
              .toStdString()
        )
    { }
};


class BlockHeaderNotFoundException: public std::runtime_error
{
public:
    BlockHeaderNotFoundException(): std::runtime_error( "Header of the block not found" ) { }
};
#endif // SUBTITLES_FIXER_EXCEPTIONS_H
