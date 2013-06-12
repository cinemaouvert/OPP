#ifndef VIDEOTRACK_H
#define VIDEOTRACK_H

#include "track.h"

class VideoTrack : public Track
{
    Q_OBJECT
public:
    explicit VideoTrack(libvlc_media_track_info_t *vlcTrackInfo, QObject *parent = 0);
    explicit VideoTrack(QObject *parent = 0);
signals:
    
public slots:
    
};

#endif // VIDEOTRACK_H
