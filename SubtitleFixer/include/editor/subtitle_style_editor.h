#ifndef SUBTITLESTYLEEDITOR_H
#define SUBTITLESTYLEEDITOR_H

#include <QStringList>

#include "reader/subtitle_block.h"


class SubtitleStyleEditor
{
public:
    SubtitleStyleEditor(SubtitleBlock &styleBlock);
    bool validate();

private:
    SubtitleBlock &_styleBlock;
    QStringList _errors;


};

#endif // SUBTITLESTYLEEDITOR_H
