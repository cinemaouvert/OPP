#include "mainwindow.h"

#include "ui_mainwindow.h"
#include "settingswindow.h"
#include "project.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // TODO: load the last project opened (in application settings)
    _currentProject = new Project();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _currentProject;
}

void MainWindow::on_action_settings_triggered()
{
    settingsWindow = new SettingsWindow(this);
    settingsWindow->show();
    settingsWindow->raise();
    settingsWindow->activateWindow();
}

