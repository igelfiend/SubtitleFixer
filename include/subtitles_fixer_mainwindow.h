#ifndef SUBTITLES_FIXER_MAINWINDOW_H
#define SUBTITLES_FIXER_MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QSettings>

class QCloseEvent;

namespace Ui {
class SubtitlesFixerMainWindow;
}

class SubtitlesFixerMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SubtitlesFixerMainWindow(QWidget *parent = nullptr);
    ~SubtitlesFixerMainWindow();

    virtual void closeEvent( QCloseEvent *event );

private slots:
    void on_pushButtonSelectFoulder_clicked();

    void on_pushButtonStart_clicked();

private:
    Ui::SubtitlesFixerMainWindow *ui;
    QSettings *_settings;
};

#endif // SUBTITLES_FIXER_MAINWINDOW_H
