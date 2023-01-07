#ifndef STRING_HELPER_H
#define STRING_HELPER_H

#include <QString>


inline bool stringIsBlockHeader(const QString &str)
{
    return str.startsWith('[') && str.endsWith(']');
}

inline QString extractBlockHeader(QString str)
{
    return str.trimmed().sliced(1, str.length() - 2);
}


#endif // STRING_HELPER_H
