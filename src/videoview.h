#ifndef VIDEOVIEW_H
#define VIDEOVIEW_H

#include <QtCore/QObject>
#include <QtGui/qwindowdefs.h>

/**
 * @interface VideoView
 * @brief The VideoView class
 */
class VideoView
{
public:
    VideoView() {}
    virtual ~VideoView() {}

    /**
     * @brief request
     * @return
     */
    virtual WId request() = 0;

    /**
     * @brief release
     */
    virtual void release() = 0;
};

#endif // VIDEOVIEW_H
