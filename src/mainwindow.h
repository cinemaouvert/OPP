#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settingswindow.h"
#include "locksettingswindow.h"

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
    void on_action_lockSettings_triggered();
    void on_action_resumeDetails_triggered();
    void on_action_automation_triggered();
    void on_action_bin_triggered();
    void on_action_statusBar_triggered();
    
private:
    Ui::MainWindow *ui;
    SettingsWindow *settingsWindow;
    LockSettingsWindow *lockSettingsWindow;
    Project *_currentProject;
};

#endif // MAINWINDOW_H
