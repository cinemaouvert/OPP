#include "advancedsettingswindow.h"
#include "ui_advancedsettingswindow.h"
#include "mediasettings.h"

AdvancedSettingsWindow::AdvancedSettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdvancedSettingsWindow),
    _mediaSettings(0)
{
    ui->setupUi(this);
}

AdvancedSettingsWindow::~AdvancedSettingsWindow()
{
    delete ui;
}
