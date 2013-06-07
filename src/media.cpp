#include "media.h"

#include "global.h"
#include "application.h"
#include <unistd.h>
#include <string.h>
#include <QThread>

Media::Media(const QString &location, libvlc_instance_t *vlcInstance, QObject *parent) :
    QObject(parent)
{
    initMedia(location);
    _vlcMedia = libvlc_media_new_path(vlcInstance, location.toLocal8Bit().data());

    // PATCH : We need to play the media to get the duration
    libvlc_media_player_t *fakeplayer = libvlc_media_player_new(vlcInstance);
    VLC_LAST_ERROR();
    libvlc_media_player_set_media(fakeplayer, _vlcMedia);
    libvlc_media_player_play(fakeplayer);
    usleep(200000);

    libvlc_track_description_t* trackAudio = libvlc_audio_get_track_description (fakeplayer);
    //qDebug()<<"Audio tracks";
    while(trackAudio)
    {
        char* s = trackAudio->psz_name;
        int i=0;
        char c=s[i];
        while(c!='\0')
        {
            qDebug()<<(int)c;
            i++;
            c=s[i];
        }
        _audioTracks.append(QPair<int, QString>(trackAudio->i_id, QString(trackAudio->psz_name)));
        qDebug()<<trackAudio->i_id;
        qDebug()<<trackAudio->psz_name;
        trackAudio=trackAudio->p_next;
    }
    //qDebug()<<"Video tracks";
    libvlc_track_description_t* trackVideo = libvlc_video_get_track_description (fakeplayer);
    while(trackVideo)
    {
        _videoTracks.append(QPair<int, QString>(trackVideo->i_id, QString(trackVideo->psz_name)));
        qDebug()<<trackVideo->i_id;
        qDebug()<<trackVideo->psz_name;
        trackVideo=trackVideo->p_next;
    }
    //qDebug()<<"Subtitles tracks";
    libvlc_track_description_t* trackSubtitles = libvlc_video_get_spu_description (fakeplayer);
    while(trackSubtitles)
    {
        _subtitlesTracks.append(QPair<int, QString>(trackSubtitles->i_id, QString(trackSubtitles->psz_name)));
        qDebug()<<trackSubtitles->i_id;
        qDebug()<<trackSubtitles->psz_name;
        trackSubtitles=trackSubtitles->p_next;
    }


    libvlc_media_player_stop(fakeplayer);
    libvlc_media_player_release(fakeplayer);

    qDebug()<<_audioTracks;
    qDebug()<<_videoTracks;
    qDebug()<<_subtitlesTracks;
}

Media::Media(const Media &media)
{
    initMedia(media.location());
    _vlcMedia = libvlc_media_duplicate(media.core());
    _audioTracks = media.audioTracks();
    _videoTracks = media.videoTracks();
    _subtitlesTracks = media.subtitlesTracks();
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

uint Media::duration() const
{
    return libvlc_media_get_duration(_vlcMedia);
}

QString Media::name() const
{
    return _fileInfo.fileName();
}

bool Media::exists() const
{
    return _fileInfo.exists();
}

QList< QPair<int, QString> > Media::audioTracks() const
{
    return _audioTracks;
}

QList< QPair<int, QString> > Media::videoTracks() const
{
    return _videoTracks;
}

QList< QPair<int, QString> > Media::subtitlesTracks() const
{
    return _subtitlesTracks;
}
