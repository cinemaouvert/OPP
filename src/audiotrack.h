#ifndef AUDIOTRACK_H
#define AUDIOTRACK_H

#include "track.h"

class AudioTrack : public Track
{
    Q_OBJECT
public:
    explicit AudioTrack(libvlc_media_track_info_t *vlcTrackInfo, QObject *parent = 0);
    explicit AudioTrack(QObject *parent = 0);

    /**
     * @brief rate Get the audio rate
     * @return Audio track rate
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    uint rate() const;    
};

#endif // AUDIOTRACK_H
