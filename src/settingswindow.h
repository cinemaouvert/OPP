#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();

    int getIndex(QString lang);
    QString getLang(int index);
    
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::SettingsWindow *ui;
};

#endif // SETTINGSWINDOW_H
