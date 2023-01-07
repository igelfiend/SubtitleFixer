#include "subtitles_fixer_mainwindow.h"
#include "ui_subtitles_fixer_mainwindow.h"

#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include <QStringList>
#include <QTextStream>
#include <cstdio>
#include <QVariant>
#include <QCloseEvent>

#include "subtitles_fixer.h"

SubtitlesFixerMainWindow::SubtitlesFixerMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SubtitlesFixerMainWindow)
    , _settings( new QSettings( "settings.ini", QSettings::IniFormat ) )
{
    ui->setupUi(this);

    ui->lineEditFoulderPath->setText( _settings->value( "path", "" ).toString() );

    ui->spinBoxIncreaseSize->setValue( _settings->value( "font_increase_value", 20 ).toInt() );
    ui->checkBoxFontsize->setChecked(  _settings->value( "font_increase_enabled", true ).toBool() );

    ui->fontComboBox->setCurrentFont( QFont( _settings->value( "font_name_value", "Calibri" ).toString() ) );
    ui->checkBoxFontName->setChecked(        _settings->value( "font_name_enabled", false ).toBool() );

    ui->comboBoxCodecs->setCurrentIndex( _settings->value( "codec_index", 0 ).toInt() );
    ui->checkBoxBomUsage->setChecked(    _settings->value( "codec_has_bom", true ).toBool() );
}

SubtitlesFixerMainWindow::~SubtitlesFixerMainWindow()
{
    delete ui;
}

void SubtitlesFixerMainWindow::closeEvent(QCloseEvent *event)
{
    _settings->setValue( "path", ui->lineEditFoulderPath->text() );

    _settings->setValue( "font_increase_value",   ui->spinBoxIncreaseSize->text() );
    _settings->setValue( "font_increase_enabled", ui->checkBoxFontsize->isChecked() );

    _settings->setValue( "font_name_value",   ui->fontComboBox->currentFont().family() );
    _settings->setValue( "font_name_enabled", ui->checkBoxFontName->isChecked() );

    _settings->setValue( "codec_index",   ui->comboBoxCodecs->currentIndex() );
    _settings->setValue( "codec_has_bom", ui->checkBoxBomUsage->isChecked()  );

    event->accept();
}

void SubtitlesFixerMainWindow::on_pushButtonSelectFoulder_clicked()
{
    QString dirPath = QFileDialog::getExistingDirectory( this,
                                                         "Select foulder with subtitles",
                                                         ui->lineEditFoulderPath->text() );

    if( dirPath.isEmpty() )
    {
        return;
    }
    else
    {
        ui->lineEditFoulderPath->setText( dirPath );
    }
}

void SubtitlesFixerMainWindow::on_pushButtonStart_clicked()
{
    QString foulderPath = ui->lineEditFoulderPath->text();

    //! Checking path for searching subtitles
    if( foulderPath.isEmpty() )
    {
        QMessageBox::critical( this, "Error", "Foulder path is empty!" );
        return;
    }

    QDir dir( foulderPath );
    if( !dir.exists() )
    {
        QMessageBox::critical( this, "Error", "Selected foulder doesn\'t exists!" );
        return;
    }

    QStringList files = dir.entryList( QStringList() << "*.ass" << "*.ASS", QDir::Files );

    //! Creating dir for new subs
    QString saveDirPath( foulderPath + "/updated_subs" );
    QDir saveDir( saveDirPath );
    if( !saveDir.exists() )
    {
        saveDir.mkpath( "." );
    }

    SubtitlesFixer fixer;

    const Settings settings = getFixerSettings();

    foreach( const QString &filename, files )
    {
        QString fullpath = foulderPath + "/" + filename;
        QString savepath = saveDirPath + "/" + filename;

        if( fixer.fixFile( fullpath, savepath ) )
        {
            ui->textEdit->append( QString( "fixed:  %1" ).arg( filename ) );
        }
        else
        {
            ui->textEdit->append( QString( "failed: %1" ).arg( filename ) );
        }
    }
}

Settings SubtitlesFixerMainWindow::getFixerSettings() const
{
    Settings settings;

    settings.setCodecSettings(
        CodecSettings{
            ui->comboBoxCodecs->currentText(),
            ui->checkBoxBomUsage->isChecked()});
    if( ui->checkBoxFontsize->isChecked() )
    {
        settings.setIncreaseFontSize( ui->spinBoxIncreaseSize->value() );
    }

    if( ui->checkBoxFontName->isChecked() )
    {
        settings.setNewFontName( ui->fontComboBox->currentFont().family() );
    }

    return settings;
}
