#ifndef SUBTITLE_ROW_H
#define SUBTITLE_ROW_H

#include <QString>
#include <QStringList>
#include <QTextStream>
#include "string_helper.h"

#define COMA_WITH_SPACES_REGEX "";


inline QString getValuesStringFromRow(const QString &row, qsizetype splitIndex)
{
    return row.last(row.length() - splitIndex - 1);
}

// It's time to start writing tests!!!!

class SubtitleRow
{
public:
    enum DataType
    {
        Unknown   = -1,
        Empty     = 0,
        Comment   = 1,
        Header    = 2,
        Content   = 3,
        Formatter = 4,
    };

    SubtitleRow()
    { }

    SubtitleRow(DataType dataType)
        : dataType(dataType)
    { }

    SubtitleRow(const QString &title, const QStringList &values, DataType dataType)
        : dataType(dataType)
        , title(title)
        , values(values)
    { }

    virtual QString toString() const
    {
        return QString();
    }

    DataType dataType;
    QString title;
    QStringList values;
};


class CommentSubtitleRow: public SubtitleRow
{
public:
    CommentSubtitleRow(const QString &row)
        : SubtitleRow(SubtitleRow::Comment)
    {
        this->title = row.last(row.length() - 1);
    }

    QString toString() const override
    {
        return QString(";%1").arg(title);
    }
};


class HeaderSubtitleRow: public SubtitleRow
{
public:
    HeaderSubtitleRow(const QString &row)
        : SubtitleRow(SubtitleRow::Header)
    {
        this->title = extractBlockHeader(row);
    }

    QString toString() const override
    {
        return QString("[%1]").arg(title);
    }
};


class ContentSubtitleRow: public SubtitleRow
{
public:
    ContentSubtitleRow(const QString &row, qsizetype splitIndex)
        : SubtitleRow(SubtitleRow::Content)
    {
        this->title = row.first(splitIndex);
        QString valuesStr = getValuesStringFromRow(row, splitIndex);
        this->values = valuesStr.split(',');
    }

    QString toString() const override
    {
        QTextStream out;
        out << title << ": " << values.join(',');
        return *out.string();
    }
};


class FormatterSubtitleRow: public SubtitleRow
{
public:
    FormatterSubtitleRow(const QString &row, qsizetype splitIndex)
        : SubtitleRow(SubtitleRow::Formatter)
    {
        this->title = row.first(splitIndex);
    }

    QString toString() const override
    {
        QTextStream out;
        out << title << ": " << values.join(", ");
        return *out.string();
    }
};


class EmptySubtitleRow: public SubtitleRow
{
public:
    EmptySubtitleRow()
        : SubtitleRow(SubtitleRow::Empty)
    { }
};


class UnknownSubtitleRow: public SubtitleRow
{
public:
    UnknownSubtitleRow(const QString &row)
        : SubtitleRow(SubtitleRow::Unknown)
    {
        this->title = row;
    }

    QString toString() const override
    {
        return title;
    }
};

#endif // SUBTITLE_ROW_H
