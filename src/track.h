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

    virtual ~Track();

    /**
     * @brief Track
     * @param track
     */
    Track(const Track &track);

    /**
     * @brief operator =
     * @param track
     * @return
     */
    Track & operator=(const Track &track);

    /**
     * @brief operator ==
     * @param track
     * @return
     */
    bool operator==(const Track &track);

    /**
     * @brief trackId
     * @return
     */
    inline int trackId() const { return _vlcTrackInfo.i_id; }

    /**
     * @brief codec
     * @return
     */
    inline uint codec() const { return _vlcTrackInfo.i_codec; }

    /**
     * @brief trackType
     * @return
     */
    inline libvlc_track_type_t trackType() const { return _vlcTrackInfo.i_type; }

    /**
     * @brief codecDescription
     * @return
     */
    QString codecDescription() const;

protected:

    /**
     * @brief _vlcTrackInfo
     */
    libvlc_media_track_info_t _vlcTrackInfo;
};

#endif // TRACK_H
