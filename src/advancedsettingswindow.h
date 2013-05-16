#ifndef ADVANCEDSETTINGSWINDOW_H
#define ADVANCEDSETTINGSWINDOW_H

#include <QMainWindow>

namespace Ui {
class AdvancedSettingsWindow;
}

class AdvancedSettingsWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit AdvancedSettingsWindow(QWidget *parent = 0);
    ~AdvancedSettingsWindow();
    
private:
    Ui::AdvancedSettingsWindow *ui;
};

#endif // ADVANCEDSETTINGSWINDOW_H
