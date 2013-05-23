#include "advancedsettingswindow.h"
#include "ui_advancedsettingswindow.h"

AdvancedSettingsWindow::AdvancedSettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdvancedSettingsWindow)
{
    ui->setupUi(this);
}

AdvancedSettingsWindow::~AdvancedSettingsWindow()
{
    delete ui;
}
