#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settingswindow.h"

namespace Ui {
class MainWindow;
}
class Project;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_action_settings_triggered();
    
private:
    Ui::MainWindow *ui;
    SettingsWindow *settingsWindow;
    Project *_currentProject;
};

#endif // MAINWINDOW_H
