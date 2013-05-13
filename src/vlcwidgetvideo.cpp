#include "vlcwidgetvideo.h"

#include <QtGlobal>
#include <QDebug>

#if QT_VERSION >= 0x050000
    #include <QtWidgets/QApplication>
    #include <QtWidgets/QDesktopWidget>
    #include <QtWidgets/QHBoxLayout>
    #include <QtWidgets/QToolBar>
#else
    #include <QtGui/QApplication>
    #include <QtGui/QDesktopWidget>
    #include <QtGui/QHBoxLayout>
    #include <QtGui/QToolBar>
#endif

#if defined(Q_OS_MAC)
    #import <Cocoa/Cocoa.h>
#elif defined(Q_WS_X11)
    #include <X11/Xlib.h>
    #include <qx11info_x11.h>
#endif

#include "vlc/Error.h"
#include "vlc/MediaPlayer.h"
#include "vlc/Video.h"

// VlcWidgetVideo::VlcWidgetVideo(VlcMediaPlayer *player,
//                                QWidget *parent)
//     :
// #if defined(Q_OS_MAC)
//       QMacCocoaViewContainer(0, parent)
// #else
//       QFrame(parent)
// #endif
// {
//     // _vlcMediaPlayer = player;
//     // 
//     // connect(_vlcMediaPlayer, SIGNAL(vout(int)), this, SLOT(applyPreviousSettings()));
// 
//     initWidgetVideo();
// }

VlcWidgetVideo::VlcWidgetVideo(QWidget *parent)
    :
#if defined(Q_OS_MAC)
      QMacCocoaViewContainer(0, parent)
#else
      QWidget(parent)
#endif
      /*,_vlcMediaPlayer(0)*/
{
    initWidgetVideo();
}

VlcWidgetVideo::~VlcWidgetVideo()
{
    release();

#if !defined(Q_OS_MAC)
    delete _layout;

    /* Ensure we are not leaking the video output. This would crash. */
    Q_ASSERT(!_video);
#endif
}

void VlcWidgetVideo::initWidgetVideo()
{
    _enableSettings = false;
    _defaultAspectRatio = Vlc::Original;
    _defaultCropRatio = Vlc::Original;
    _defaultDeinterlacing = Vlc::Disabled;
    _defaultScale = Vlc::NoScale;
    _currentAspectRatio = Vlc::Original;
    _currentCropRatio = Vlc::Original;
    _currentDeinterlacing = Vlc::Disabled;
    _currentScale = Vlc::NoScale;

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

void VlcWidgetVideo::setMediaPlayer(/*VlcMediaPlayer *player*/)
{
    // _vlcMediaPlayer = player;
    // 
    // connect(_vlcMediaPlayer, SIGNAL(vout(int)), this, SLOT(applyPreviousSettings()));
}

void VlcWidgetVideo::setCurrentAspectRatio(const Vlc::Ratio &ratio)
{
    _currentAspectRatio = ratio;
}

void VlcWidgetVideo::setCurrentCropRatio(const Vlc::Ratio &ratio)
{
    _currentCropRatio = ratio;
}

void VlcWidgetVideo::setCurrentDeinterlacing(const Vlc::Deinterlacing &deinterlacing)
{
    _currentDeinterlacing = deinterlacing;
}

void VlcWidgetVideo::setCurrentScale(const Vlc::Scale &scale)
{
    _currentScale = scale;
}

void VlcWidgetVideo::setDefaultAspectRatio(const Vlc::Ratio &ratio)
{
    _defaultAspectRatio = ratio;
}

void VlcWidgetVideo::setDefaultCropRatio(const Vlc::Ratio &ratio)
{
    _defaultCropRatio = ratio;
}

void VlcWidgetVideo::setDefaultDeinterlacing(const Vlc::Deinterlacing &deinterlacing)
{
    _defaultDeinterlacing = deinterlacing;
}

void VlcWidgetVideo::setDefaultScale(const Vlc::Scale &scale)
{
    _defaultScale = scale;
}

void VlcWidgetVideo::enableDefaultSettings()
{
    initDefaultSettings();

    enablePreviousSettings();
}

void VlcWidgetVideo::enablePreviousSettings()
{
    _enableSettings = true;
}

void VlcWidgetVideo::initDefaultSettings()
{
    _currentAspectRatio = defaultAspectRatio();
    _currentCropRatio = defaultCropRatio();
    _currentDeinterlacing = defaultDeinterlacing();
    _currentScale = defaultScale();
}

void VlcWidgetVideo::applyPreviousSettings()
{
    if (!_enableSettings)
        return;

    // if (!_vlcMediaPlayer)
    //     return;
    // 
    // _vlcMediaPlayer->video()->setAspectRatio(_currentAspectRatio);
    // _vlcMediaPlayer->video()->setCropGeometry(_currentCropRatio);
    // _vlcMediaPlayer->video()->setScale(_currentScale);
    // _vlcMediaPlayer->video()->setDeinterlace(_currentDeinterlacing);

    _enableSettings = false;
}

void VlcWidgetVideo::setAspectRatio(const Vlc::Ratio &ratio)
{
    // if (_vlcMediaPlayer) {
    //     _currentAspectRatio = ratio;
    //     _vlcMediaPlayer->video()->setAspectRatio(ratio);
    // }
}

void VlcWidgetVideo::setCropRatio(const Vlc::Ratio &ratio)
{
    // if (_vlcMediaPlayer) {
    //     _currentCropRatio = ratio;
    //     _vlcMediaPlayer->video()->setCropGeometry(ratio);
    // }
}

void VlcWidgetVideo::setDeinterlacing(const Vlc::Deinterlacing &deinterlacing)
{
    // if (_vlcMediaPlayer) {
    //     _currentDeinterlacing = deinterlacing;
    //     _vlcMediaPlayer->video()->setDeinterlace(deinterlacing);
    // }
}

void VlcWidgetVideo::setScale(const Vlc::Scale &scale)
{
    // if (_vlcMediaPlayer) {
    //     _currentScale = scale;
    //     _vlcMediaPlayer->video()->setScale(scale);
    // }
}

void VlcWidgetVideo::sync()
{
#if defined(Q_WS_X11)
    /* Make sure the X server has processed all requests.
     * This protects other threads using distinct connections from getting
     * the video widget window in an inconsistent states. */
    XSync(QX11Info::display(), False);
#endif
}

WId VlcWidgetVideo::request()
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
    /* Indicates that the widget wants to draw directly onto the screen.
       Widgets with this attribute set do not participate in composition
       management */
    /* This is currently disabled on X11 as it does not seem to improve
     * performance, but causes the video widget to be transparent... */
# ifndef Q_WS_X11
	_video->setAttribute( Qt::WA_PaintOnScreen, true );
# endif
    _layout->addWidget(_video);
    sync();
	
    return _video->winId();
#endif
}

void VlcWidgetVideo::release()
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
