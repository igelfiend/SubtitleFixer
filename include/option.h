#ifndef OPTION_H

template< typename T >
/**
 * @brief The FixField class contains 2 fields:
 *      - value with field value;
 *      - enabled flag, used in process functions.
 */
class Option
{
//! Public members
public:
    bool enabled;
    T value;

//! Public methods
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
