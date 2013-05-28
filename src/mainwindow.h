#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class SettingsWindow;
class LockSettingsWindow;
class Application;

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
    Application *_app;
    SettingsWindow *_settingsWindow;
    LockSettingsWindow *lockSettingsWindow;
};

#endif // MAINWINDOW_H
