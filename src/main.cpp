#include "subtitles_fixer_mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SubtitlesFixerMainWindow w;
    w.show();

    return a.exec();
}
