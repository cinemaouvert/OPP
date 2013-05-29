#include "videowindow.h"

#include "videowidget.h"

VideoWindow::VideoWindow(QWidget *parent) :
    QMainWindow(parent)
{
    _videoWidget = new VideoWidget(this);
    setCentralWidget(_videoWidget);
    setWindowTitle("Video");
    resize(640,480);
}

VideoWindow::~VideoWindow()
{
    delete _videoWidget;
}
