#ifndef AUDIOTRACK_H
#define AUDIOTRACK_H

#include "track.h"

class AudioTrack : public Track
{
    Q_OBJECT
public:
    explicit AudioTrack(libvlc_media_track_info_t *vlcTrackInfo, QObject *parent = 0);
    explicit AudioTrack(QObject *parent = 0);

signals:
    
public slots:
    
};

#endif // AUDIOTRACK_H
