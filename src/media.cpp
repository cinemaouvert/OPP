#include "media.h"

#include "global.h"
#include "application.h"

Media::Media(const QString &location, libvlc_instance_t *vlcInstance, QObject *parent) :
    QObject(parent),
    _location(location)
{
    initMedia(location, vlcInstance);
}

Media::Media(const Media &media)
{
    _location = media.location();
    _fileInfo = QFileInfo(_location);
    _vlcMedia = libvlc_media_duplicate(media.core());
}

Media::~Media()
{
    libvlc_media_release(_vlcMedia);
}

void Media::initMedia(const QString &location, libvlc_instance_t *vlcInstance)
{
    QString l = location;
    l.replace("/", "\\");
    _fileInfo = QFileInfo(location);
    _vlcMedia = libvlc_media_new_path(vlcInstance, l.toLocal8Bit().data());
}

Media & Media::operator=(const Media &media)
{
    if (this != &media) {
        _location = media.location();
        _fileInfo = QFileInfo(_location);
        _vlcMedia = libvlc_media_duplicate(media.core());
    }
    return *this;
}

bool Media::operator==(const Media &media) const
{
    return _location == media.location();
}

QString Media::name() const
{
    return _fileInfo.fileName();
}

bool Media::exists() const
{
    return _fileInfo.exists();
}
