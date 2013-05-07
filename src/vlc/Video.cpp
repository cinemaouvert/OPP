/****************************************************************************
* VLC-Qt - Qt and libvlc connector library
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
*
* This library is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <vlc/vlc.h>

#include "Error.h"
#include "MediaPlayer.h"
#include "Video.h"

VlcVideo::VlcVideo(VlcMediaPlayer *player)
    : QObject(player),
      _vlcMediaPlayer(player->core()) { }

VlcVideo::~VlcVideo() { }

Vlc::Ratio VlcVideo::aspectRatio() const
{
    QString ratio = "";
    if (_vlcMediaPlayer && libvlc_media_player_has_vout(_vlcMediaPlayer)) {
        ratio = libvlc_video_get_aspect_ratio(_vlcMediaPlayer);
        VlcError::errmsg();
    }

    return Vlc::Ratio(Vlc::ratio().indexOf(ratio));
}

Vlc::Ratio VlcVideo::cropGeometry() const
{
    QString crop = "";
    if (_vlcMediaPlayer && libvlc_media_player_has_vout(_vlcMediaPlayer)) {
        crop = libvlc_video_get_crop_geometry(_vlcMediaPlayer);
        VlcError::errmsg();
    }

    return Vlc::Ratio(Vlc::ratio().indexOf(crop));
}

void VlcVideo::hideLogo()
{
    if (_vlcMediaPlayer && libvlc_media_player_has_vout(_vlcMediaPlayer)) {
        libvlc_video_set_logo_int(_vlcMediaPlayer, libvlc_logo_enable, 0);
        VlcError::errmsg();
    }
}

void VlcVideo::hideMarquee()
{
    if (_vlcMediaPlayer && libvlc_media_player_has_vout(_vlcMediaPlayer)) {
        libvlc_video_set_marquee_int(_vlcMediaPlayer, libvlc_marquee_Enable, 0);
        VlcError::errmsg();
    }
}

Vlc::Scale VlcVideo::scale() const
{
    float scale = 0;
    if (_vlcMediaPlayer && libvlc_media_player_has_vout(_vlcMediaPlayer)) {
        scale = libvlc_video_get_scale(_vlcMediaPlayer);
        VlcError::errmsg();
    }

    return Vlc::Scale(Vlc::scale().indexOf(scale));
}

void VlcVideo::setAspectRatio(const Vlc::Ratio &ratio)
{
    if (_vlcMediaPlayer && libvlc_media_player_has_vout(_vlcMediaPlayer)) {
        libvlc_video_set_aspect_ratio(_vlcMediaPlayer, Vlc::ratio()[ratio].toLocal8Bit().data());
        VlcError::errmsg();
    }
}

void VlcVideo::setCropGeometry(const Vlc::Ratio &ratio)
{
    if (_vlcMediaPlayer && libvlc_media_player_has_vout(_vlcMediaPlayer)) {
        libvlc_video_set_crop_geometry(_vlcMediaPlayer, Vlc::ratio()[ratio].toLocal8Bit().data());
        VlcError::errmsg();
    }
}

void VlcVideo::setDeinterlace(const Vlc::Deinterlacing &filter)
{
    if (_vlcMediaPlayer && libvlc_media_player_has_vout(_vlcMediaPlayer)) {
        libvlc_video_set_deinterlace(_vlcMediaPlayer, Vlc::deinterlacing()[filter].toLocal8Bit().data());
        VlcError::errmsg();
    }
}

void VlcVideo::setScale(const Vlc::Scale &scale)
{
    if (_vlcMediaPlayer && libvlc_media_player_has_vout(_vlcMediaPlayer)) {
        libvlc_video_set_scale(_vlcMediaPlayer, Vlc::scale()[scale]);
        VlcError::errmsg();
    }
}

void VlcVideo::setSubtitle(const int &subtitle)
{
    if (_vlcMediaPlayer && libvlc_media_player_has_vout(_vlcMediaPlayer)) {
        libvlc_video_set_spu(_vlcMediaPlayer, subtitle);
        VlcError::errmsg();
    }
}

void VlcVideo::setSubtitleFile(const QString &subtitle)
{
    if (_vlcMediaPlayer && libvlc_media_player_has_vout(_vlcMediaPlayer)) {
        libvlc_video_set_subtitle_file(_vlcMediaPlayer, subtitle.toLocal8Bit().data());
        VlcError::errmsg();
    }
}

void VlcVideo::setTeletextPage(const int &page)
{
    if (_vlcMediaPlayer && libvlc_media_player_has_vout(_vlcMediaPlayer)) {
        libvlc_video_set_teletext(_vlcMediaPlayer, page);
        VlcError::errmsg();
    }
}

void VlcVideo::setTrack(const int &track)
{
    if (_vlcMediaPlayer) {
        libvlc_video_set_track(_vlcMediaPlayer, track);
        VlcError::errmsg();
    }
}

void VlcVideo::showLogo(const QString &file,
                        const int &x,
                        const int &y,
                        const int &opacity)
{
    if (_vlcMediaPlayer && libvlc_media_player_has_vout(_vlcMediaPlayer)) {
        libvlc_video_set_logo_string(_vlcMediaPlayer, libvlc_logo_file, file.toLocal8Bit().data());
        libvlc_video_set_logo_int(_vlcMediaPlayer, libvlc_logo_x, x);
        libvlc_video_set_logo_int(_vlcMediaPlayer, libvlc_logo_y, y);
        libvlc_video_set_logo_int(_vlcMediaPlayer, libvlc_logo_opacity, opacity);

        libvlc_video_set_logo_int(_vlcMediaPlayer, libvlc_logo_enable, 1);
    }
}

void VlcVideo::showMarquee(const QString &text,
                           const int &x,
                           const int &y,
                           const int &timeout,
                           const int &opacity,
                           const int &size,
                           const int &color)
{
    if (_vlcMediaPlayer && libvlc_media_player_has_vout(_vlcMediaPlayer)) {
        libvlc_video_set_marquee_string(_vlcMediaPlayer, libvlc_marquee_Text, text.toLocal8Bit().data());
        libvlc_video_set_marquee_int(_vlcMediaPlayer, libvlc_marquee_X, x);
        libvlc_video_set_marquee_int(_vlcMediaPlayer, libvlc_marquee_Y, y);
        libvlc_video_set_marquee_int(_vlcMediaPlayer, libvlc_marquee_Timeout, timeout);
        libvlc_video_set_marquee_int(_vlcMediaPlayer, libvlc_marquee_Opacity, opacity);
        libvlc_video_set_marquee_int(_vlcMediaPlayer, libvlc_marquee_Size, size);
        libvlc_video_set_marquee_int(_vlcMediaPlayer, libvlc_marquee_Color, color);

        libvlc_video_set_marquee_int(_vlcMediaPlayer, libvlc_marquee_Enable, 1);
    }
}

QSize VlcVideo::size() const
{
    unsigned x;
    unsigned y;

    if (_vlcMediaPlayer && libvlc_media_player_has_vout(_vlcMediaPlayer)) {
        libvlc_video_get_size(_vlcMediaPlayer, 0, &x, &y);
        VlcError::errmsg();
    }

    return QSize(x, y);
}

int VlcVideo::subtitle() const
{
    int subtitle = -1;
    if (_vlcMediaPlayer && libvlc_media_player_has_vout(_vlcMediaPlayer)) {
        subtitle = libvlc_video_get_spu(_vlcMediaPlayer);
        VlcError::errmsg();
    }

    return subtitle;
}

int VlcVideo::subtitleCount() const
{
    int count = -1;
    if (_vlcMediaPlayer && libvlc_media_player_has_vout(_vlcMediaPlayer)) {
        count = libvlc_video_get_spu_count(_vlcMediaPlayer);
        VlcError::errmsg();
    }

    return count;
}

QStringList VlcVideo::subtitleDescription() const
{
    QStringList descriptions;

    if (_vlcMediaPlayer && libvlc_media_player_has_vout(_vlcMediaPlayer)) {
        libvlc_track_description_t *desc;
        desc = libvlc_video_get_spu_description(_vlcMediaPlayer);
        VlcError::errmsg();

        descriptions << QString().fromUtf8(desc->psz_name);
        if (subtitleCount() > 1) {
            for(int i = 1; i < subtitleCount(); i++) {
                desc = desc->p_next;
                descriptions << QString().fromUtf8(desc->psz_name);
            }
        }
    }

    return descriptions;
}

QList<int> VlcVideo::subtitleIds() const
{
    QList<int> ids;

    if (_vlcMediaPlayer && libvlc_media_player_has_vout(_vlcMediaPlayer)) {
        libvlc_track_description_t *desc;
        desc = libvlc_video_get_spu_description(_vlcMediaPlayer);
        VlcError::errmsg();

        ids << desc->i_id;
        if (subtitleCount() > 1) {
            for(int i = 1; i < trackCount(); i++) {
                desc = desc->p_next;
                ids << desc->i_id;
            }
        }
    }

    return ids;
}

bool VlcVideo::takeSnapshot(const QString &path) const
{
    bool success = false;
    if (_vlcMediaPlayer && libvlc_media_player_has_vout(_vlcMediaPlayer)) {
        success = libvlc_video_take_snapshot(_vlcMediaPlayer, 0, path.toLocal8Bit().data(), 0, 0) + 1;
        VlcError::errmsg();
    }

    return success;
}

int VlcVideo::teletextPage() const
{
    int page = -1;
    if (_vlcMediaPlayer && libvlc_media_player_has_vout(_vlcMediaPlayer)) {
        page = libvlc_video_get_teletext(_vlcMediaPlayer);
        VlcError::errmsg();
    }

    return page;
}

void VlcVideo::toggleTeletextTransparency()
{
    if (_vlcMediaPlayer && libvlc_media_player_has_vout(_vlcMediaPlayer)) {
        libvlc_toggle_teletext(_vlcMediaPlayer);
        VlcError::errmsg();
    }
}

int VlcVideo::track() const
{
    int track = -1;
    if (_vlcMediaPlayer) {
        track = libvlc_video_get_track(_vlcMediaPlayer);
        VlcError::errmsg();
    }

    return track;
}

int VlcVideo::trackCount() const
{
    int count = -1;
    if (_vlcMediaPlayer) {
        count = libvlc_video_get_track_count(_vlcMediaPlayer);
        VlcError::errmsg();
    }

    return count;
}

QStringList VlcVideo::trackDescription() const
{
    QStringList descriptions;

    if (_vlcMediaPlayer) {
        libvlc_track_description_t *desc;
        desc = libvlc_video_get_track_description(_vlcMediaPlayer);
        VlcError::errmsg();

        descriptions << QString().fromUtf8(desc->psz_name);
        if (trackCount() > 1) {
            for(int i = 1; i < trackCount(); i++) {
                desc = desc->p_next;
                descriptions << QString().fromUtf8(desc->psz_name);
            }
        }
    }

    return descriptions;
}

QList<int> VlcVideo::trackIds() const
{
    QList<int> ids;

    if (_vlcMediaPlayer) {
        libvlc_track_description_t *desc;
        desc = libvlc_video_get_track_description(_vlcMediaPlayer);
        VlcError::errmsg();

        ids << desc->i_id;
        if (trackCount() > 1) {
            for(int i = 1; i < trackCount(); i++) {
                desc = desc->p_next;
                ids << desc->i_id;
            }
        }
    }

    return ids;
}
