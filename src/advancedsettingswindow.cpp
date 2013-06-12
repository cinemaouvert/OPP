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
