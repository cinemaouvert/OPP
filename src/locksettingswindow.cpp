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
    //_locker->setPasswordEnable(true);
     ui->lineEdit_password->setEnabled(true);
}

void LockSettingsWindow::on_radioButton_passwordOff_clicked()
{
    //_locker->setPasswordEnable(false);
    ui->lineEdit_password->setEnabled(false);
}

void LockSettingsWindow::on_radioButton_autoLockOn_clicked()
{
    //_locker->setAutoLock(true);
}

void LockSettingsWindow::on_radioButton_autoLockOff_clicked()
{
   // _locker->setAutoLock(false);
}

void LockSettingsWindow::on_lineEdit_password_editingFinished()
{
    //_locker->setPassword(ui->lineEdit_password->text());
}

void LockSettingsWindow::on_buttonBox_accepted()
{
    /*Password*/
    _locker->setPasswordEnable(ui->radioButton_passwordOn->isChecked());
    if(ui->radioButton_passwordOn->isChecked())
        _locker->setPassword(ui->lineEdit_password->text());

    /*Automatic lock*/
    _locker->setAutoLock(ui->radioButton_autoLockOn->isChecked());
    _locker->setAutoLockDelay(ui->spinBox_delay->value());

    this->hide();
}

void LockSettingsWindow::on_buttonBox_rejected()
{
    this->hide();
}

void LockSettingsWindow::setLocker()
{
    /*Password*/
    if(_locker->passwordEnable())
    {
        ui->radioButton_passwordOn->setChecked(true);
        ui->radioButton_passwordOff->setChecked(false);
        ui->lineEdit_password->setEnabled(true);
    }
    else
    {
        ui->radioButton_passwordOn->setChecked(false);
        ui->radioButton_passwordOff->setChecked(true);
        ui->lineEdit_password->setEnabled(false);
    }
    ui->lineEdit_password->setText(_locker->getPassword());
    /*Automatic lock*/
    if(_locker->getAutoLock())
    {
        ui->radioButton_autoLockOn->setChecked(true);
        ui->radioButton_autoLockOff->setChecked(false);
    }
    else
    {
        ui->radioButton_autoLockOn->setChecked(false);
        ui->radioButton_autoLockOff->setChecked(true);
    }
    ui->spinBox_delay->setValue(0);

}
