#ifndef VIDEOVIEW_H
#define VIDEOVIEW_H

class VideoView {

public:

    VideoView() {};

    virtual ~VideoView() {};

    virtual WId request() = 0;

    virtual void release() = 0;
};

#endif // VIDEOVIEW_H
