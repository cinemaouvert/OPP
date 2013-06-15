#ifndef VIDEOTRACK_H
#define VIDEOTRACK_H

#include <QSize>

#include "track.h"

class VideoTrack : public Track
{
    Q_OBJECT
public:
    explicit VideoTrack(libvlc_media_track_info_t *vlcTrackInfo, QObject *parent = 0);
    explicit VideoTrack(QObject *parent = 0);

    /**
     * @brief size
     * @return
     */
    QSize size() const;

    /**
     * @brief width
     * @return
     */
    uint width() const;

    /**
     * @brief height
     * @return
     */
    uint height() const;
};

#endif // VIDEOTRACK_H
