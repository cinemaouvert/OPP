#include "advancedpicturesettingswindow.h"
#include "ui_advancedpicturesettingswindow.h"

AdvancedPictureSettingsWindow::AdvancedPictureSettingsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdvancedPictureSettingsWindow)
{
    ui->setupUi(this);
}

AdvancedPictureSettingsWindow::~AdvancedPictureSettingsWindow()
{
    delete ui;
}
