#include "advancedsettingswindow.h"
#include "ui_advancedsettingswindow.h"
#include "playback.h"
#include "mainwindow.h"
#include "utils.h"

AdvancedSettingsWindow::AdvancedSettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdvancedSettingsWindow),
    _playback(0)
{
    ui->setupUi(this);
}

AdvancedSettingsWindow::~AdvancedSettingsWindow()
{
    delete ui;
}

void AdvancedSettingsWindow::setPlayback(Playback* playback)
{
    _playback=playback;
    /*Movie title*/
    ui->label_title->setText(_playback->media()->name());

    /*Setting in and out mark*/
    QTime timeIn = msecToQTime(_playback->mediaSettings()->inMark());
    ui->timeEdit_inMark->setTime(timeIn);
    QTime timeOut = msecToQTime(_playback->mediaSettings()->outMark());
    ui->timeEdit_outMark->setTime(timeOut);

    /*Original length*/
    QTime original =  msecToQTime(_playback->media()->duration());
    ui->label_originalLengthValue->setText(original.toString("hh:mm:ss"));

    /*Modified length*/
    updateLength();


    /*Fill table*/
    while (ui->tableWidget_information->rowCount() > 0)
    {
        ui->tableWidget_information->removeRow(0);
    }
    ui->tableWidget_information->insertRow(0);
    QTableWidgetItem* itemTitle = new QTableWidgetItem( tr("Title") );
    ui->tableWidget_information->setItem( 0, 0, itemTitle );
    itemTitle = new QTableWidgetItem(_playback->media()->name());
    ui->tableWidget_information->setItem( 0, 1, itemTitle);
    int line=1;
    QTableWidgetItem* itemTrack;
    foreach(VideoTrack videoTrack, _playback->media()->videoTracks())
    {
        int row = ui->tableWidget_information->rowCount();
        ui->tableWidget_information->insertRow(row);
        itemTrack = new QTableWidgetItem(QString(tr("Track %1 (Video)")).arg(QString::number(videoTrack.trackId())));
        ui->tableWidget_information->setItem(line, 0, itemTrack);
        itemTrack = new QTableWidgetItem(videoTrack.codecDescription());
        ui->tableWidget_information->setItem(line, 1, itemTrack);

        line++;
    }
    foreach(AudioTrack audioTrack, _playback->media()->audioTracks())
    {
        int row = ui->tableWidget_information->rowCount();
        ui->tableWidget_information->insertRow(row);
        itemTrack = new QTableWidgetItem(QString(tr("Track %1 (Audio)")).arg(QString::number(audioTrack.trackId())));
        ui->tableWidget_information->setItem(line, 0, itemTrack);
        itemTrack = new QTableWidgetItem(audioTrack.codecDescription());
        ui->tableWidget_information->setItem(line, 1, itemTrack);

        line++;
    }
    foreach(Track subtitlesTrack, _playback->media()->subtitlesTracks())
    {
        int row = ui->tableWidget_information->rowCount();
        ui->tableWidget_information->insertRow(row);
        itemTrack = new QTableWidgetItem(QString(tr("Track %1 (Subtitles)")).arg(QString::number(subtitlesTrack.trackId())));
        ui->tableWidget_information->setItem(line, 0, itemTrack);
        itemTrack = new QTableWidgetItem(subtitlesTrack.codecDescription());
        ui->tableWidget_information->setItem(line, 1, itemTrack);

        line++;
    }

}

void AdvancedSettingsWindow::on_buttonBox_OKCancel_accepted()
{
    /*Test pattern*/
    int index = ui->comboBox_testPattern->currentIndex();
    if (index == -1)
        return;
    _playback->mediaSettings()->setTestPattern(index==0);

    /*In mark and out mark*/
    _playback->mediaSettings()->setInMark(qTimeToMsec(ui->timeEdit_inMark->time()));
    _playback->mediaSettings()->setOutMark(qTimeToMsec(ui->timeEdit_outMark->time()));

    this->hide();
}

void AdvancedSettingsWindow::on_buttonBox_OKCancel_rejected()
{
    this->hide();
}

void AdvancedSettingsWindow::updateLength()
{
    int secIn = ui->timeEdit_inMark->time().second()+60*ui->timeEdit_inMark->time().minute()+3600*ui->timeEdit_inMark->time().hour();
    int secOut = ui->timeEdit_outMark->time().second()+60*ui->timeEdit_outMark->time().minute()+3600*ui->timeEdit_outMark->time().hour();
    int diff = 1000*(secOut-secIn);
    QTime modified =  msecToQTime(diff);
    ui->label_modifiedLengthValue->setText(modified.toString("hh:mm:ss"));
}

void AdvancedSettingsWindow::on_timeEdit_inMark_timeChanged(const QTime &date)
{
    updateLength();
}

void AdvancedSettingsWindow::on_timeEdit_outMark_timeChanged(const QTime &date)
{
    updateLength();
}
