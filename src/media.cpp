#include "media.h"

#include "global.h"
#include "application.h"

Media::Media(const QString &location, libvlc_instance_t *vlcInstance, QObject *parent) :
    QObject(parent)
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
    _location = location;
    _fileInfo = QFileInfo(location);
    _vlcMedia = libvlc_media_new_path(vlcInstance, location.toLocal8Bit().data());
    libvlc_media_parse(_vlcMedia);
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

QTime Media::duration() const
{
    libvlc_time_t msecs = libvlc_media_get_duration(_vlcMedia);

    const int hours = msecs/(1000*60*60);
    const int minutes = (msecs-(hours*1000*60*60))/(1000*60);
    const int seconds = (msecs-(minutes*1000*60)-(hours*1000*60*60))/1000;

    return QTime(hours,minutes,seconds);
}

QString Media::name() const
{
    return _fileInfo.fileName();
}

bool Media::exists() const
{
    return _fileInfo.exists();
}
