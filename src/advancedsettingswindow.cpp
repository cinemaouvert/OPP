#include "advancedsettingswindow.h"
#include "ui_advancedsettingswindow.h"
#include "playback.h"
#include "mainwindow.h"

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
    ui->label_title->setText(_playback->media()->name());
}

void AdvancedSettingsWindow::on_buttonBox_OKCancel_accepted()
{
    this->hide();
}

void AdvancedSettingsWindow::on_buttonBox_OKCancel_rejected()
{
    this->hide();
}
