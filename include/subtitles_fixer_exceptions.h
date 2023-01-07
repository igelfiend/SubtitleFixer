#ifndef SUBTITLES_FIXER_EXCEPTIONS_H
#define SUBTITLES_FIXER_EXCEPTIONS_H

#include <stdexcept>
#include <string>

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
    ConvertionToIntFailedException(): std::runtime_error( "Failed while trying to convert to int" ) { }
};


class FileNotOpenedException: public std::runtime_error
{
public:
    FileNotOpenedException(): std::runtime_error( "File is not opened to be read" ) { }
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

#endif // SUBTITLES_FIXER_EXCEPTIONS_H
