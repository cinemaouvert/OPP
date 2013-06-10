#ifndef LOCKSETTINGSWINDOW_H
#define LOCKSETTINGSWINDOW_H

#include <QDialog>

namespace Ui {
class LockSettingsWindow;
}
class Locker;

class LockSettingsWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit LockSettingsWindow(Locker *locker, QWidget *parent = 0);
    ~LockSettingsWindow();
    
private slots:
    void on_radioButton_passwordOn_clicked();

    void on_radioButton_passwordOff_clicked();

    void on_radioButton_autoLockOn_clicked();

    void on_radioButton_autoLockOff_clicked();

    void on_lineEdit_password_editingFinished();

private:
    Ui::LockSettingsWindow *ui;
    Locker *_locker;
};

#endif // LOCKSETTINGSWINDOW_H
