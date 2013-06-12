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
    void updateLength();
    
private slots:
    void on_buttonBox_OKCancel_accepted();

    void on_buttonBox_OKCancel_rejected();

    void on_timeEdit_inMark_timeChanged(const QTime &date);

    void on_timeEdit_outMark_timeChanged(const QTime &date);

private:
    Ui::AdvancedSettingsWindow *ui;
    Playback* _playback;
};

#endif // ADVANCEDSETTINGSWINDOW_H
