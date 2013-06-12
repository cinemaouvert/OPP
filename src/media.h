#ifndef MEDIA_H
#define MEDIA_H

#include <QtCore/QObject>
#include <QFileInfo>
#include <QTime>
#include <QPair>
#include <QSize>

#include "audiotrack.h"
#include "videotrack.h"
#include "track.h"

struct libvlc_media_t;
struct libvlc_instance_t;

class Media : public QObject
{
    Q_OBJECT
public:
    Media(const QString &location, libvlc_instance_t *vlcInstance, QObject *parent = 0);
    Media(const Media &media);
    virtual ~Media();

    void initMedia(const QString &location);

    Media & operator=(const Media &media);
    bool operator==(const Media &media) const;

    inline const QString & location() const { return _location; }
    inline libvlc_media_t* core() const { return _vlcMedia; }
    inline QSize videoResolution() const { return _videoResolution; }

    uint duration() const;
    QString name() const;

    bool exists() const;

    QList<AudioTrack> audioTracks() const;
    QList<VideoTrack> videoTracks() const;
    QList<Track> subtitlesTracks() const;

    QStringList audioTracksName() const;
    QStringList videoTracksName() const;
    QStringList subtitlesTracksName() const;

    bool isImage() const;

    void usageCountAdd(int count = 1);
    bool isUsed() const;

protected:
    void parseMediaInfos();

signals:
    void usageCountChanged();

private:
    QString _location;
    QFileInfo _fileInfo;

    libvlc_media_t *_vlcMedia;

    QList<AudioTrack> _audioTracks;
    QList<VideoTrack> _videoTracks;
    QList<Track> _subtitlesTracks;

    int _usageCount;
    QSize _videoResolution;

    friend QDataStream & operator << (QDataStream &, const QList<Media> &);
    friend QDataStream & operator >> (QDataStream &, QList<Media> &);
};

#endif // MEDIA_H
