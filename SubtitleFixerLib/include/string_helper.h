#ifndef STRING_HELPER_H
#define STRING_HELPER_H

#include <QDebug>
#include <QRegularExpression>
#include <QString>
#include <QStringList>
#include <QStringView>

#define COMA_WITH_SPACES_REGEX "(\\,\\s*)"

inline bool stringIsBlockHeader(const QString &str)
{
    QStringView strView = str.trimmed();
    return strView.startsWith('[') && strView.endsWith(']');
}

inline QString extractBlockHeader(QString str)
{
    QStringView strView = str.trimmed();
    return strView.trimmed().sliced(1, strView.length() - 2).toString();
}

struct RowData
{
    QString title;
    QStringList values;
};

inline RowData getRowDataFromString(const QString &str)
{
    QStringView strView = QStringView(str).trimmed();
    qsizetype splitIndex = strView.indexOf(':');

    if( splitIndex < 0 )
    {
        qCritical() << "getRowDataFromString: trying to extract data from unappropriate string \"" << str << "\"";
        throw std::runtime_error( "incorrect string for extracting data" );
    }

    RowData rowData;
    rowData.title = strView.first(splitIndex).toString();

    QString valuesStr = strView.last(strView.length() - splitIndex - 1).trimmed().toString();
    rowData.values = valuesStr.split(QRegularExpression(COMA_WITH_SPACES_REGEX));

    return rowData;
}

#endif // STRING_HELPER_H
