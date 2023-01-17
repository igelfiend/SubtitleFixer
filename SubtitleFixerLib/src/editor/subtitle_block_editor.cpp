#include <QDebug>

#include "editor/subtitle_block_editor.h"
#include "editor/subtitle_block_validator.h"
#include "subtitles_fixer_exceptions.h"

SubtitleBlockEditor::SubtitleBlockEditor(SubtitleBlock &block)
    : _block(block)
{
    initFormatIndexes();
}

void SubtitleBlockEditor::validate()
{
    SubtitleBlockValidator validator(_block);
    if( !validator.validate() )
    {
        qCritical() << "SubtitleBlockEditor: provided style block is not valid. Error: "
                    << validator.getErrors();
    }
}

QStringList SubtitleBlockEditor::getFormatterFields() const
{
    if( _block.hasFormatLine() )
    {
        qCritical() << "SubtitleBlockEditor: trying to get format line from block without it";
        throw FormatterRowNotFoundException();
    }

    SubtitleRowPtr formatLine = _block.getFormatLine();
    return formatLine->getValues();
}

void SubtitleBlockEditor::initFormatIndexes()
{
    QStringList columnTitles = getFormatterFields();
    for( qsizetype i = 0; i < columnTitles.length(); ++i )
    {
        _columnTitlesIndexes[ columnTitles[ i ] ] = i;
    }
}

qsizetype SubtitleBlockEditor::getColumnIndex(const QString &columnName) const
{
    if( _columnTitlesIndexes.find( columnName ) == _columnTitlesIndexes.end() )
    {
        throw ColumnNameNotFromFormatLineException( columnName );
    }

    return _columnTitlesIndexes[ columnName ];
}

void SubtitleBlockEditor::updateAllValuesForField(
    const QString &fieldName, std::function<QString (const QString&)> updater)
{
    qsizetype columnIndex = getColumnIndex( fieldName );
    for( auto lineIt = _block.lines.begin(); lineIt != _block.lines.end(); ++lineIt )
    {
        const QString &elementToUpdate = (*lineIt)->getValues().at( columnIndex );

        QString updatedValue = updater( elementToUpdate );

        (*lineIt)->setValue( columnIndex, updatedValue );
    }
}
