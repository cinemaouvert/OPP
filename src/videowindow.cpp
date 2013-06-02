#include "videowindow.h"

#include <QApplication>
#include <QDesktopWidget>

#include "videowidget.h"

VideoWindow::VideoWindow(QWidget *parent) :
    QMainWindow(parent)
{
    _videoWidget = new VideoWidget(this);
    setCentralWidget(_videoWidget);

    setWindowTitle("Video");
    resize(640,480);

    setDisplayMode(WINDOW);
}

VideoWindow::~VideoWindow()
{
    delete _videoWidget;
}

void VideoWindow::setDisplayMode(const DisplayMode &mode)
{
    _mode = mode;

    switch (_mode) {
    case PROJECTION:
        moveToDisplay(1);
        showFullScreen();
        break;
    case WINDOW:
        moveToDisplay(0);
        showNormal();
        break;
    default:
        void;
    }
}

void VideoWindow::moveToDisplay(const int &display)
{
    QRect secondDisplayRes = QApplication::desktop()->screenGeometry(display);

    move(QPoint(secondDisplayRes.x(), secondDisplayRes.y()));
}
