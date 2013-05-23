#include "locksettingswindow.h"
#include "ui_locksettingswindow.h"

LockSettingsWindow::LockSettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LockSettingsWindow)
{
    ui->setupUi(this);
}

LockSettingsWindow::~LockSettingsWindow()
{
    delete ui;
}
