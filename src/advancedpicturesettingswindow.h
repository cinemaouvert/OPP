#ifndef ADVANCEDPICTURESETTINGSWINDOW_H
#define ADVANCEDPICTURESETTINGSWINDOW_H

#include <QDialog>

namespace Ui {
class AdvancedPictureSettingsWindow;
}

class Playback;
class AdvancedPictureSettingsWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit AdvancedPictureSettingsWindow(QWidget *parent = 0);
    ~AdvancedPictureSettingsWindow();
    void setPlayback(Playback* playback);
    
private slots:
    void on_comboBox_deinterlace_currentIndexChanged(int index);

    void on_buttonBox_OKCancel_accepted();

    void on_buttonBox_OKCancel_rejected();

private:
    Ui::AdvancedPictureSettingsWindow *ui;
    Playback* _playback;
};

#endif // ADVANCEDPICTURESETTINGSWINDOW_H
