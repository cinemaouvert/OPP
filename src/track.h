#ifndef TRACK_H
#define TRACK_H

#include <QObject>

#include <vlc/vlc.h>

class Track : public QObject
{
    Q_OBJECT            
public:

    explicit Track(libvlc_media_track_info_t *vlcTrackInfo, QObject *parent = 0);

    explicit Track(QObject *parent = 0);

    Track(const Track &track);

    virtual ~Track();

    Track & operator=(const Track &track);

    bool operator==(const Track &track);

    inline int trackId() const { return _vlcTrackInfo.i_id; }

    inline uint codec() const { return _vlcTrackInfo.i_codec; }

    inline libvlc_track_type_t trackType() const { return _vlcTrackInfo.i_type; }

    QString codecDescription() const;

protected:

    libvlc_media_track_info_t _vlcTrackInfo;

};

#endif // TRACK_H
