#ifndef OPTION_H

template< typename T >
/**
 * @brief Option template class store both value and enable/disable flag
 */
class Option
{
public:
    bool enabled;
    T value;

public:
    Option()
        : enabled( false )
        , value()
    { }

    Option( const T &value )
        : enabled( true )
        , value( value )
    { }
};

#define OPTION_H

#endif // OPTION_H
