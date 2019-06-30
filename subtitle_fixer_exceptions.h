#ifndef SUBTITLE_FIXER_EXCEPTIONS_H
#define SUBTITLE_FIXER_EXCEPTIONS_H

#include <stdexcept>
#include <string>

class NoSuchFileException: public std::runtime_error
{
public:
    NoSuchFileException(): std::runtime_error( "File with such name not found" ) { }
};

class OpenningFileErrorException: public std::runtime_error
{
public:
    OpenningFileErrorException(): std::runtime_error( "Failed while openning file" ) { }
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


#endif // SUBTITLE_FIXER_EXCEPTIONS_H
