#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "settingswindow.h"
#include "locksettingswindow.h"

#include "application.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _app = new Application();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_settings_triggered()
{
    _settingsWindow = new SettingsWindow(this);
    _settingsWindow->show();
    _settingsWindow->raise();
    _settingsWindow->activateWindow();
}

void MainWindow::on_action_lockSettings_triggered()
{
    lockSettingsWindow = new LockSettingsWindow(this);
    lockSettingsWindow->show();
    lockSettingsWindow->raise();
    lockSettingsWindow->activateWindow();
}

void MainWindow::on_action_resumeDetails_triggered()
{
    if(ui->action_resumeDetails->isChecked())
        ui->resumeDetailWidget->show();
    else
        ui->resumeDetailWidget->hide();
}

void MainWindow::on_action_automation_triggered()
{
    if(ui->action_automation->isChecked())
        ui->automationWidget->show();
    else
        ui->automationWidget->hide();
}

void MainWindow::on_action_bin_triggered()
{
    if(ui->action_bin->isChecked())
        ui->binWidget->show();
    else
        ui->binWidget->hide();
}

void MainWindow::on_action_statusBar_triggered()
{
    if(ui->action_statusBar->isChecked())
        ui->StatusBar->show();
    else
        ui->StatusBar->hide();
}

