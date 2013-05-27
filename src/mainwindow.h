#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class SettingsWindow;
class Application;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

//    void updateApplicationContext();

private slots:
    void on_action_settings_triggered();
    
private:
    Ui::MainWindow *ui;
    SettingsWindow *_settingsWindow;
    Application *_app;
};

#endif // MAINWINDOW_H
