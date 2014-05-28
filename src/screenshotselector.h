#ifndef SCREENSHOTSELECTOR_H
#define SCREENSHOTSELECTOR_H

#include <QDialog>
#include "media.h"
#include <vlc/vlc.h>


namespace Ui {
class ScreenshotSelector;
}

class ScreenshotSelector : public QDialog
{
    Q_OBJECT
    
public:
    explicit ScreenshotSelector(QWidget *parent = 0);
    ~ScreenshotSelector();

    /**
      * @brief used to set the filename
      *
      */
    void setMedia(Media *media);
    
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_seekSlider_sliderReleased();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_stepInput_valueChanged(int arg1);

private:
    Ui::ScreenshotSelector *ui;
    libvlc_instance_t *vlc;
    libvlc_media_player_t *vlcp;
    WId _currentWId;
    Media *_media;

    void close();
    void closeEvent (QCloseEvent *event);
    void setScreenshot(int position);


};

#endif // SCREENSHOTSELECTOR_H
