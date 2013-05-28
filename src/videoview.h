#ifndef VIDEOVIEW_H
#define VIDEOVIEW_H

#include <QtCore/QObject>
#include <QtGui/qwindowdefs.h>

class VideoView {

public:

    VideoView() {}

    virtual ~VideoView() {}

    virtual WId request() = 0;

    virtual void release() = 0;
};

#endif // VIDEOVIEW_H
