#ifndef MEDIA_H
#define MEDIA_H

#include <QtCore/QObject>
#include <QFileInfo>
#include <QTime>

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

private:
    QString _location;
    QFileInfo _fileInfo;
    libvlc_media_t *_vlcMedia;

    friend QDataStream & operator << (QDataStream &, const QList<Media> &);
    friend QDataStream & operator >> (QDataStream &, QList<Media> &);
};

#endif // MEDIA_H
