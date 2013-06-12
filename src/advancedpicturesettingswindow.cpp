#include "advancedpicturesettingswindow.h"
#include "ui_advancedpicturesettingswindow.h"
#include <QDebug>

#include "playback.h"

#include "mainwindow.h"

AdvancedPictureSettingsWindow::AdvancedPictureSettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdvancedPictureSettingsWindow),
    _playback(0)
{
    ui->setupUi(this);

    ui->comboBox_deinterlace->addItems(MediaSettings::deinterlacingValues());
}

AdvancedPictureSettingsWindow::~AdvancedPictureSettingsWindow()
{
    delete ui;
}

void AdvancedPictureSettingsWindow::on_comboBox_deinterlace_currentIndexChanged(int index)
{
    //_mediaPlayer->setCurrentDeinterlacing((Deinterlacing) index);
}

void AdvancedPictureSettingsWindow::setPlayback(Playback* playback)
{
    _playback=playback;
    ui->comboBox_deinterlace->setCurrentIndex(_playback->mediaSettings()->deinterlacing());
}

void AdvancedPictureSettingsWindow::on_buttonBox_OKCancel_accepted()
{
    /*Setting deinterlace*/
    int indexDeinterlacing = ui->comboBox_deinterlace->currentIndex();
    if (indexDeinterlacing == -1)
        return;
    Deinterlacing deinterlacing = (Deinterlacing)indexDeinterlacing;
    _playback->mediaSettings()->setDeinterlacing(deinterlacing);


    this->hide();
}

void AdvancedPictureSettingsWindow::on_buttonBox_OKCancel_rejected()
{
    this->hide();
}
