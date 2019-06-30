# SubtitleFixer
Subtitle fixer is software for mass updating subtitles.
Current state: works with *.ass files.
Usage:
* Select foulder with *.ass subtitles;
* Set increase step for font size. Value can be setted as negative for decreasing font size value. That step will be applied to all subtitle styles in file. Can be turned off with nearby checkbox "enabled";
* Set new font name. That step will be applied to all subtitle styles in file. Can be turned off with nearby checkbox "enabled";
* Choose codec if required (default codec used is UTF-8 with BOM);
* Press "Start fix" button

If fix was successfull, you will see message "fixed: <filepath>", otherwise text of the message will be "failed: <filepath>". Any troubles can be watched through console error messages.
