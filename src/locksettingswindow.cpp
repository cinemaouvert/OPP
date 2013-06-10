#include "locksettingswindow.h"
#include "ui_locksettingswindow.h"

#include "locker.h"

LockSettingsWindow::LockSettingsWindow(Locker *locker, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LockSettingsWindow),
    _locker(locker)
{
    ui->setupUi(this);
    //_locker->setPasswordEnable(false);
}

LockSettingsWindow::~LockSettingsWindow()
{
    delete ui;
}

void LockSettingsWindow::on_radioButton_passwordOn_clicked()
{
    _locker->setPasswordEnable(true);
     ui->lineEdit_password->setEnabled(true);
}

void LockSettingsWindow::on_radioButton_passwordOff_clicked()
{
    _locker->setPasswordEnable(false);
    ui->lineEdit_password->setEnabled(false);
}

void LockSettingsWindow::on_radioButton_autoLockOn_clicked()
{
    _locker->setAutoLock(true);
}

void LockSettingsWindow::on_radioButton_autoLockOff_clicked()
{
    _locker->setAutoLock(false);
}

void LockSettingsWindow::on_lineEdit_password_editingFinished()
{
    _locker->setPassword(ui->lineEdit_password->text());
}
