#ifndef LOCKSETTINGSWINDOW_H
#define LOCKSETTINGSWINDOW_H

#include <QDialog>

namespace Ui {
class LockSettingsWindow;
}

class LockSettingsWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit LockSettingsWindow(QWidget *parent = 0);
    ~LockSettingsWindow();
    
private:
    Ui::LockSettingsWindow *ui;
};

#endif // LOCKSETTINGSWINDOW_H
