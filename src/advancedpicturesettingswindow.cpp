#include "advancedpicturesettingswindow.h"
#include "ui_advancedpicturesettingswindow.h"

#include "mediasettings.h"

#include "mainwindow.h"

AdvancedPictureSettingsWindow::AdvancedPictureSettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdvancedPictureSettingsWindow)
{
    ui->setupUi(this);


    ui->comboBox_deinterlace->addItems(MediaSettings::deinterlacingValues());
    ui->comboBox_deinterlace->removeItem(0);
}

AdvancedPictureSettingsWindow::~AdvancedPictureSettingsWindow()
{
    delete ui;
}

void AdvancedPictureSettingsWindow::on_comboBox_deinterlace_currentIndexChanged(int index)
{
    //_mediaPlayer->setCurrentDeinterlacing((Deinterlacing) index);
}
