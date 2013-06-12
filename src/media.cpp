#include "media.h"

#include "global.h"
#include "application.h"
#include <unistd.h>
#include <string.h>
#include <QThread>

Media::Media(const QString &location, libvlc_instance_t *vlcInstance, QObject *parent) :
    QObject(parent),
    _usageCount(0)
{
    initMedia(location);
    _vlcMedia = libvlc_media_new_path(vlcInstance, location.toLocal8Bit().data());
    parseMediaInfos();
    /*
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
        _audioTracks.append(QPair<int, QString>(trackAudio->i_id, QString::fromUtf8(trackAudio->psz_name)));
        qDebug()<<trackAudio->i_id;
        qDebug()<<trackAudio->psz_name;
        trackAudio=trackAudio->p_next;
    }
    //qDebug()<<"Video tracks";
    libvlc_track_description_t* trackVideo = libvlc_video_get_track_description (fakeplayer);
    while(trackVideo)
    {
        QString track;
        track=track.fromUtf8(trackVideo->psz_name);
        _videoTracks.append(QPair<int, QString>(trackVideo->i_id, track));
        qDebug()<<trackVideo->i_id;
        qDebug()<<trackVideo->psz_name;
        trackVideo=trackVideo->p_next;
    }
    //qDebug()<<"Subtitles tracks";
    libvlc_track_description_t* trackSubtitles = libvlc_video_get_spu_description (fakeplayer);
    while(trackSubtitles)
    {
        QString track;
        track=track.fromUtf8(trackSubtitles->psz_name);
        _subtitlesTracks.append(QPair<int, QString>(trackSubtitles->i_id, track));
        qDebug()<<trackSubtitles->i_id;
        qDebug()<<trackSubtitles->psz_name;
        trackSubtitles=trackSubtitles->p_next;
    }


    libvlc_media_player_stop(fakeplayer);
    libvlc_media_player_release(fakeplayer);

    qDebug()<<_audioTracks;
    qDebug()<<_videoTracks;
    qDebug()<<_subtitlesTracks;
    */
}

Media::Media(const Media &media)
{
    initMedia(media._location);
    _usageCount = media._usageCount;
    _vlcMedia = libvlc_media_duplicate(media._vlcMedia);
}

Media::~Media()
{
    libvlc_media_release(_vlcMedia);
}

void Media::parseMediaInfos()
{
    // TODO : use parse async + send signal when media is parsed
    //libvlc_media_parse(_vlcMedia);

    // @see libvlc_media_get_tracks_info
    // @see libvlc_media_tracks_get
    // @see libvlc_media_tracks_release
    // http://www.videolan.org/developers/vlc/doc/doxygen/html/group__libvlc__media.html#ga2102c151df0ab66d6158a80b7734f0f9

//    libvlc_media_track_info_t track[20 /*get tracks number ?*/];
//    libvlc_media_get_tracks_info(_vlcMedia, &track);
    libvlc_media_parse(_vlcMedia);
    _audioTracks.append(-1);
    _videoTracks.append(-1);
    _subtitlesTracks.append(-1);

    libvlc_media_track_info_t* tracks;

    int tracksCount = libvlc_media_get_tracks_info(_vlcMedia, &tracks);
    for(int track=0;track<tracksCount;track++)
    {
        switch(tracks[track].i_type)
        {
            case libvlc_track_audio : //Audio track
                _audioTracks.append(tracks[track].i_id);
                break;
            case libvlc_track_video : //Video track
                _videoTracks.append(tracks[track].i_id);
                break;
            case libvlc_track_text : //Subtitles track
                _subtitlesTracks.append(tracks[track].i_id);
                break;
        }
    }

    qDebug()<<"=======================================================================";
    qDebug()<<"length : "<<duration();
    qDebug()<<"audio : "<<_audioTracks;
    qDebug()<<"video : "<<_videoTracks;
    qDebug()<<"subtitles : "<<_subtitlesTracks;
    qDebug()<<"=======================================================================";
}

void Media::initMedia(const QString &location)
{
    _location = location;
    _fileInfo = QFileInfo(location);
}

Media & Media::operator=(const Media &media)
{
    if (this != &media) {
        _location = media._location;
        _fileInfo = QFileInfo(_location);
        _vlcMedia = libvlc_media_duplicate(media._vlcMedia);
//        _usageCount = media._usageCount;
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

QList<int> Media::audioTracks() const
{
    return _audioTracks;
}

void Media::usageCountAdd(int count)
{
    _usageCount += count;
    emit usageCountChanged();
}

bool Media::isImage() const
{
    return _audioTracks.count() == 1 && _videoTracks.count() == 2 && _subtitlesTracks.count() == 1;
}

bool Media::isUsed() const
{
    return _usageCount > 0;
}

QList<int> Media::videoTracks() const
{
    return _videoTracks;
}

QList<int> Media::subtitlesTracks() const
{
    return _subtitlesTracks;
}
