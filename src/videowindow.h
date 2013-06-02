#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include <QMainWindow>

#include "videowidget.h"

class VideoWindow : public QMainWindow
{
    Q_OBJECT
public:

    enum DisplayMode {
        /**
         * Video onto a classical window
         */
        WINDOW = 0,

        /**
         * Fullscreen video into the 2nd display
         */
        PROJECTION = 1
    };

    explicit VideoWindow(QWidget *parent = 0);
    ~VideoWindow();

    inline VideoWidget* videoWidget() const { return _videoWidget; }

    void setDisplayMode(const DisplayMode &mode);

    void moveToDisplay(const int &display);

private:
    VideoWidget *_videoWidget;
    DisplayMode _mode;
};

#endif // VIDEOWINDOW_H
