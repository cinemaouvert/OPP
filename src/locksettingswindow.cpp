#include "locksettingswindow.h"
#include "ui_locksettingswindow.h"

#include "locker.h"

LockSettingsWindow::LockSettingsWindow(Locker *locker, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LockSettingsWindow),
    _locker(locker)
{
    ui->setupUi(this);
}

LockSettingsWindow::~LockSettingsWindow()
{
    delete ui;
}
