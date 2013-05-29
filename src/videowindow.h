#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include <QMainWindow>

#include "videowidget.h"

class VideoWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit VideoWindow(QWidget *parent = 0);
    ~VideoWindow();

    inline VideoWidget* videoWidget() const { return _videoWidget; }

private:
    VideoWidget *_videoWidget;
};

#endif // VIDEOWINDOW_H
