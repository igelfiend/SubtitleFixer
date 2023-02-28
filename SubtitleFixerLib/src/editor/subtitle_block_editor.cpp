#include <QDebug>

#include "editor/subtitle_block_editor.h"
#include "subtitles_fixer_exceptions.h"


SubtitleBlockEditor::SubtitleBlockEditor(SubtitleBlock &block, ValidatorPtr validatorPtr)
    : _block( block )
    , _validatorPtr( (validatorPtr == nullptr) ? QSharedPointer<SubtitleBlockValidator>::create() : validatorPtr )
{
    initFormatIndexes();
}

void SubtitleBlockEditor::validate()
{
    if( !_validatorPtr->validate(_block) )
    {
        qCritical() << "SubtitleBlockEditor: provided style block is not valid. Error: "
                    << _validatorPtr->getErrors();
    }
}

QStringList SubtitleBlockEditor::getFormatterFields() const
{
    return _block.getFormatLine()->getValues();
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
    auto dataLines = _block.getDataLines();
    qsizetype columnIndex = getColumnIndex( fieldName );
    for( auto line: dataLines )
    {
        const QString &elementToUpdate = line->getValues().at( columnIndex );
        QString updatedValue = updater( elementToUpdate );

        line->setValue( columnIndex, updatedValue );
    }
}
