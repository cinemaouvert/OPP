#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QMainWindow>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();
    
private:
    Ui::SettingsWindow *ui;
};

#endif // SETTINGSWINDOW_H
