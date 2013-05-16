#include "advancedsettingswindow.h"
#include "ui_advancedsettingswindow.h"

AdvancedSettingsWindow::AdvancedSettingsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdvancedSettingsWindow)
{
    ui->setupUi(this);
}

AdvancedSettingsWindow::~AdvancedSettingsWindow()
{
    delete ui;
}
