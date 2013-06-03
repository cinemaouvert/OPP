#include "media.h"

#include "global.h"
#include "application.h"

Media::Media(const QString &location, libvlc_instance_t *vlcInstance, QObject *parent) :
    QObject(parent)
{
    initMedia(location);
    _vlcMedia = libvlc_media_new_path(vlcInstance, location.toLocal8Bit().data());

    // PATCH : We need to play the media to get the duration
    libvlc_media_player_t *fakeplayer = libvlc_media_player_new(vlcInstance);
    libvlc_media_player_set_media(fakeplayer, _vlcMedia);
    libvlc_media_player_play(fakeplayer);
    libvlc_media_player_stop(fakeplayer);
    libvlc_media_player_release(fakeplayer);
}

Media::Media(const Media &media)
{
    initMedia(media.location());
    _vlcMedia = libvlc_media_duplicate(media.core());
}

Media::~Media()
{
    libvlc_media_release(_vlcMedia);
}

void Media::initMedia(const QString &location)
{
    _location = location;
    _fileInfo = QFileInfo(location);
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
