#include "videowidget.h"

#include <QtGlobal>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QToolBar>
#include <QPainter>

#if defined(Q_OS_MAC)
    #import <Cocoa/Cocoa.h>
#elif defined(Q_WS_X11)
    #include <X11/Xlib.h>
    #include <qx11info_x11.h>
#endif

VideoWidget::VideoWidget(QWidget *parent)
    :
#if defined(Q_OS_MAC)
      QMacCocoaViewContainer(0, parent)
#else
      QWidget(parent)
#endif
{
    init();
}

VideoWidget::~VideoWidget()
{
    release();

#if !defined(Q_OS_MAC)
    delete _layout;

    /* Ensure we are not leaking the video output. This would crash. */
    Q_ASSERT(!_video);
#endif
}

void VideoWidget::init()
{
#if defined(Q_OS_MAC)
    NSView *video = [[NSView alloc] init];
    setCocoaView(video);
    [video release];
#else
    _layout = new QHBoxLayout(this);
    _layout->setContentsMargins(0, 0, 0, 0);
    _video = 0;

    QPalette plt = palette();
    plt.setColor(QPalette::Window, Qt::black);
    setPalette(plt);
#endif
}

void VideoWidget::sync()
{
#if defined(Q_WS_X11)
    /* Make sure the X server has processed all requests.
     * This protects other threads using distinct connections from getting
     * the video widget window in an inconsistent states. */
    XSync(QX11Info::display(), False);
#endif
}

WId VideoWidget::request()
{
#if defined(Q_OS_MAC)
    return WId(cocoaView());
#else
    if (_video)
        return 0;

    _video = new QWidget();
    QPalette plt = palette();
    plt.setColor(QPalette::Window, Qt::black);
    _video->setPalette(plt);
    _video->setAutoFillBackground(true);
    _video->setMouseTracking(true);
# ifndef Q_WS_X11
    _video->setAttribute( Qt::WA_PaintOnScreen, true );
# endif
    _layout->addWidget(_video);
    sync();

    return _video->winId();
#endif
}

void VideoWidget::release()
{
#if !defined(Q_OS_MAC)
    if (_video) {
        _layout->removeWidget(_video);
        _video->deleteLater();
        _video = NULL;
    }
#endif
    updateGeometry();
}
