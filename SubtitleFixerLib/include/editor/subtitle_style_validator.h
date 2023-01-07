#ifndef SUBTITLE_STYLE_VALIDATOR_H
#define SUBTITLE_STYLE_VALIDATOR_H

#include "reader/subtitle_block.h"


class SubtitleStyleValidator
{
public:
    SubtitleStyleValidator(const SubtitleBlock &styleBlock)
        : _styleBlock(styleBlock)
    { }

    const SubtitleBlock &getBlockForValidation() const
    {
        return _styleBlock;
    }

private:
    const SubtitleBlock &_styleBlock;
};


#endif // SUBTITLE_STYLE_VALIDATOR_H
