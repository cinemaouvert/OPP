#ifndef VIDEOWIDGET_H_
#define VIDEOWIDGET_H_

#include <QtCore/QTimer>

#if QT_VERSION >= 0x050000
    #include <QtWidgets/QFrame>
#else
    #include <QtGui/QFrame>
#endif

#if defined(Q_OS_MAC)
    #include <QMacCocoaViewContainer>
#endif

#include "videoview.h"

#if defined(Q_OS_MAC)
class VideoWidget : public QMacCocoaViewContainer, public VideoView
#else
class VideoWidget : public QWidget, public VideoView
#endif
{
Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent = 0);

    ~VideoWidget();

    WId request();

    void release();

private:
    void init();

    void sync();

#if !defined(Q_OS_MAC)
    QWidget *_video;
    QLayout *_layout;
#endif
};

#endif // VIDEOWIDGET_H_
