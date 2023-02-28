#ifndef SUBTITLE_BLOCK_EDITOR_H
#define SUBTITLE_BLOCK_EDITOR_H

#include <QHash>
#include <QString>
#include <QStringList>
#include <functional>

#include "reader/subtitle_block.h"
#include "editor/subtitle_block_validator.h"


class SubtitleBlockEditor
{
public:
    SubtitleBlockEditor(SubtitleBlock &block, ValidatorPtr validatorPtr = nullptr);
    void validate();

    QStringList getFormatterFields() const;

    void updateAllValuesForField(
            const QString &fieldName,
            std::function<QString(const QString &element)> updater
    );

private:
    qsizetype getColumnIndex( const QString &columnName ) const;
    void initFormatIndexes();

    SubtitleBlock &_block;
    ValidatorPtr _validatorPtr;
    QHash<QString, qsizetype> _columnTitlesIndexes;
};

#endif // SUBTITLE_BLOCK_EDITOR_H
