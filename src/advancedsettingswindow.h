#ifndef ADVANCEDSETTINGSWINDOW_H
#define ADVANCEDSETTINGSWINDOW_H

#include <QDialog>

namespace Ui {
class AdvancedSettingsWindow;
}
class Playback;
class AdvancedSettingsWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit AdvancedSettingsWindow(QWidget *parent = 0);
    ~AdvancedSettingsWindow();
    void setPlayback(Playback* playback);
    
private slots:
    void on_buttonBox_OKCancel_accepted();

    void on_buttonBox_OKCancel_rejected();

private:
    Ui::AdvancedSettingsWindow *ui;
    Playback* _playback;
};

#endif // ADVANCEDSETTINGSWINDOW_H
