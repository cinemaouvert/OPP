#include "media.h"

#include <unistd.h>
#include <string.h>

#include <QStringList>

#include <vlc/plugins/vlc_common.h>
#include <vlc/plugins/vlc_interface.h>

#include "global.h"
#include "application.h"

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
    libvlc_media_parse(_vlcMedia);
qDebug()<<"=======================================================================";
//    _audioTracks.append(-1);
//    _videoTracks.append(-1);
//    _subtitlesTracks.append(-1);

    libvlc_media_track_info_t* tracks;
    int tracksCount = libvlc_media_get_tracks_info(_vlcMedia, &tracks);

    for (int track = 0; track < tracksCount; track++) {
//        if (tracks[track].i_type == libvlc_track_audio) {
//            AudioTrack audio(tracks + track);
//        }
//        qDebug() << tracks[track].i_profile;
        switch (tracks[track].i_type)
        {
            case libvlc_track_audio:
                _audioTracks << AudioTrack( &tracks[track] );
                break;
            case libvlc_track_video:
                _videoTracks << VideoTrack( &tracks[track] );

//                _videoResolution.setWidth(tracks[track].u.video.i_width);
//                _videoResolution.setHeight(tracks[track].u.video.i_height);
                break;
            case libvlc_track_text:
//                _subtitlesTracks.append(tracks[track].i_id);
                _subtitlesTracks << Track( &tracks[track] );
                break;
        }
    }

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

QList<AudioTrack> Media::audioTracks() const
{
    return _audioTracks;
}

QList<VideoTrack> Media::videoTracks() const
{
    return _videoTracks;
}

QList<Track> Media::subtitlesTracks() const
{
    return _subtitlesTracks;
}

QStringList Media::audioTracksName() const
{
    QStringList list;

    foreach (const AudioTrack &track, _audioTracks)
        list << ("Track " + QString::number(track.trackId()));

    return list;
}

QStringList Media::videoTracksName() const
{
    QStringList list;

    foreach (const VideoTrack &track, _videoTracks)
        list << ("Track " + QString::number(track.trackId()));

    return list;
}

QStringList Media::subtitlesTracksName() const
{
    QStringList list;

    foreach (const Track &track, _subtitlesTracks)
        list << ("Track " + QString::number(track.trackId()));

    return list;
}

QStringList Media::audioExtensions()
{
    return QString(EXTENSIONS_AUDIO).split(";");
}

QStringList Media::videoExtensions()
{
    return QString(EXTENSIONS_VIDEO).split(";");
}

QStringList Media::imageExtensions()
{
    return QString(EXTENSIONS_IMAGE).split(";");
}

QStringList Media::mediaExtensions()
{
    return Media::audioExtensions() + Media::videoExtensions() + Media::imageExtensions();
}
