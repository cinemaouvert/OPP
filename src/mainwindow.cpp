#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingswindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_settings_triggered()
{
    settingsWindow = new SettingsWindow(this);
    settingsWindow->show();
    settingsWindow->raise();
    settingsWindow->activateWindow();
}

