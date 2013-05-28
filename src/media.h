#ifndef MEDIA_H
#define MEDIA_H

#include <QtCore/QObject>
#include <QFileInfo>

struct libvlc_media_t;
struct libvlc_instance_t;

class Media : public QObject
{
    Q_OBJECT
public:
    Media(const QString &location, libvlc_instance_t *vlcInstance, QObject *parent = 0);
    Media(const Media &media);
    virtual ~Media();

    void initMedia(const QString &location, libvlc_instance_t *vlcInstance);

    Media & operator=(const Media &media);
    bool operator==(const Media &media) const;

    inline const QString & location() const { return _location; }
    inline libvlc_media_t* core() const { return _vlcMedia; }

    QString name() const;

    bool exists() const;

private:
    QString _location;
    QFileInfo _fileInfo;
    libvlc_media_t *_vlcMedia;
};

#endif // MEDIA_H
