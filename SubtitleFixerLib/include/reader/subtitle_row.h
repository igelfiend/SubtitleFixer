#ifndef SUBTITLE_ROW_H
#define SUBTITLE_ROW_H

#include <QSharedPointer>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include "string_helper.h"

class ISubtitleRow
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

    virtual ~ISubtitleRow() { }

    virtual QString toString() const = 0;
    virtual bool isStoreValues() const = 0;
    virtual DataType getDataType() const = 0;
    virtual const QString &getTitle() const = 0;
    virtual const QStringList &getValues() const = 0;
    virtual bool operator==( const ISubtitleRow&) const = 0;
    virtual bool operator!=( const ISubtitleRow&) const = 0;
};

typedef QSharedPointer< ISubtitleRow > SubtitleRowPtr;

class SubtitleRow: public ISubtitleRow
{
public:
    SubtitleRow(const QString &title, const QStringList &values, DataType dataType)
        : dataType(dataType)
        , title(title)
        , values(values)
    { }

    QString toString() const override
    {
        return QString();
    }

    bool isStoreValues() const override
    {
        return false;
    }

    DataType getDataType() const override
    {
        return dataType;
    }

    const QString &getTitle() const override
    {
        return title;
    }

    const QStringList &getValues() const override
    {
        return values;
    }

    bool operator==(const ISubtitleRow &another) const override
    {
        return ( this->dataType == another.getDataType() )
                && ( this->title == another.getTitle() )
                && (this->values == another.getValues() );
    }

    bool operator!=(const ISubtitleRow &another) const override
    {
        return !( *this == another );
    }

    DataType dataType;
    QString title;
    QStringList values;

protected:
    SubtitleRow()
    { }
    SubtitleRow(DataType dataType)
        : dataType(dataType)
    { }
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

    bool isStoreValues() const override
    {
        return false;
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

    bool isStoreValues() const override
    {
        return false;
    }
};


class ContentSubtitleRow: public SubtitleRow
{
public:
    ContentSubtitleRow(const QString &row)
        : SubtitleRow(SubtitleRow::Content)
    {
        RowData rowData = getRowDataFromString(row);
        this->title = rowData.title;
        this->values = rowData.values;
    }

    QString toString() const override
    {
        return QString( "%1: %2" ).arg( title, values.join(',') );
    }

    bool isStoreValues() const override
    {
        return true;
    }
};


class FormatterSubtitleRow: public ContentSubtitleRow
{
public:
    FormatterSubtitleRow(const QString &row)
        : ContentSubtitleRow(row)
    {
        this->dataType = SubtitleRow::Formatter;
    }

    QString toString() const override
    {
        return QString( "%1: %2" ).arg( title, values.join(", ") );
    }
};


class EmptySubtitleRow: public SubtitleRow
{
public:
    EmptySubtitleRow()
        : SubtitleRow(SubtitleRow::Empty)
    { }

    QString toString() const override
    {
        return QString();
    }

    bool isStoreValues() const override
    {
        return false;
    }
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

    bool isStoreValues() const override
    {
        return false;
    }
};

#endif // SUBTITLE_ROW_H
