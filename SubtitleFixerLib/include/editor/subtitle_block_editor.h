#ifndef SUBTITLE_BLOCK_EDITOR_H
#define SUBTITLE_BLOCK_EDITOR_H

#include <QHash>
#include <QString>
#include <QStringList>
#include <functional>

#include "reader/subtitle_block.h"


class SubtitleBlockEditor
{
public:
    SubtitleBlockEditor(SubtitleBlock &block);
    void validate();

    QStringList getFormatterFields() const;
    qsizetype getColumnIndex( const QString &columnName ) const;

//    template<typename T> void updateAllValuesForField(const QString &fieldName, T newValue);
    void updateAllValuesForField(
            const QString &fieldName,
            std::function<QString(const QString &element)> updater
    );

private:
    void initFormatIndexes();

    SubtitleBlock &_block;
    QHash<QString, qsizetype> _columnTitlesIndexes;
};

#endif // SUBTITLE_BLOCK_EDITOR_H
