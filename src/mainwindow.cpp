#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "settingswindow.h"
#include "application.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _app = Application::instance();
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

