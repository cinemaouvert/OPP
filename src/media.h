#ifndef MEDIA_H
#define MEDIA_H

#include <QtCore/QObject>
#include <QFileInfo>
#include <QTime>
#include <QPair>

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

    uint duration() const;
    QString name() const;

    bool exists() const;

    QList<int> audioTracks() const;
    QList<int> videoTracks() const;
    QList<int> subtitlesTracks() const;

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

    QList<int> _audioTracks;
    QList<int> _videoTracks;
    QList<int> _subtitlesTracks;

    int _usageCount;

    friend QDataStream & operator << (QDataStream &, const QList<Media> &);
    friend QDataStream & operator >> (QDataStream &, QList<Media> &);
};

#endif // MEDIA_H
