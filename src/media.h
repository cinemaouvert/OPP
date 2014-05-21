/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
 *
 * Open Projection Program is an initiative of Catalogue Ouvert du Cinéma.
 * The software was developed by four students of University of Poitiers
 * as school project.
 *
 * Open Projection Program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Open Projection Program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Open Projection Program. If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************************/

#ifndef MEDIA_H
#define MEDIA_H

#define EXTENSIONS_AUDIO "*.3ga;*.669;*.a52;*.aac;*.ac3;*.adt;*.adts;*.aif;*.aifc;*.aiff;*.amr;*.aob;" \
    "*.ape;*.awb;*.caf;*.dts;*.flac;*.it;*.kar;*.m4a;*.m4p;*.m5p;*.mid;*.mka;*.mlp;*.mod;*.mpa;*.mp1;*.mp2;" \
    "*.mp3;*.mpc;*.mpga;*.oga;*.ogg;*.oma;*.opus;*.qcp;*.ra;*.rmi;*.s3m;*.spx;*.thd;*.tta;*.voc;*.vqf;*.w64;" \
    "*.wav;*.wma;*.wv;*.xa;*.xm"

#define EXTENSIONS_VIDEO "*.3g2;*.3gp;*.3gp2;*.3gpp;*.amv;*.asf;*.avi;*.bin;*.divx;*.drc;*.dv;*f4v;*.flv;*.gvi;*.gvi;*.gxf;*.iso;*.m1v;*.m2v;" \
                         "*.m2t;*.m2ts;*.m4v;*.mkv;*.mov;*.mp2;*.mp2v;*.mp4;*.mp4v;*.mpe;*.mpeg;*.mpeg1;" \
                         "*.mpeg2;*.mpeg4;*.mpg;*.mpv2;*.mts;*.mtv;*.mxf;*.mxg;*.nsv;*.nuv;" \
                         "*.ogg;*.ogm;*.ogv;*.ogx;*.ps;" \
                         "*.rec;*.rm;*.rmvb;*.tod;*.ts;*.tts;*.vob;*.vro;*.webm;*.wm;*.wmv;*.wtv;*.xesc"

#define EXTENSIONS_IMAGE "*.png;*.jpeg;*.jpg;*.gif;*.tiff"

#include <QtCore/QObject>
#include <QFileInfo>
#include <QTime>
#include <QPair>
#include <QSize>

#include "audiotrack.h"
#include "videotrack.h"
#include "track.h"

struct libvlc_media_t;
struct libvlc_instance_t;

/**
 * @brief Manage media informations
 */
class Media : public QObject
{
    Q_OBJECT
public:
    Media(const QString &location, libvlc_instance_t *vlcInstance, QObject *parent = 0, bool isFile = true);
    Media(const Media &media);
    virtual ~Media();

    /**
     * @brief Initialize a media from location
     * @param location The media location, it must be a valid path
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void initMedia(const QString &location);

    /**
     * @brief Overload = operator
     * @param media A media instance to copy
     * @return this
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    Media & operator=(const Media &media);

    /**
     * @brief operator ==
     * @param media
     * @return true if both match, false otherwise
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    bool operator==(const Media &media) const;

    /**
     * @brief Set identifier
     * @param id The new identifier
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setId(int id);

    /**
     * @brief Get current location
     * @return The location
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline const QString & location() const { return _location; }

    /**
     * @brief Get libvlc media core
     * @return The libvlc media core
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline libvlc_media_t* core() const { return _vlcMedia; }

    /**
     * @brief Get identifier
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline int id() const { return _id; }

    /**
     * @brief Get duration
     * @return The duration in ms
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    uint duration() const;

    /**
     * @brief Get file name
     * @return The file name
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    QString name() const;

    /**
     * @brief File exists in the filesystem
     * @return True if file exists, false otherwise
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    bool exists() const;

    /**
     * @brief Get the list of audio tracks
     * @return The list of audio tracks. If no audio track was found, it returns an empty list
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    QList<AudioTrack> audioTracks() const;

    /**
     * @brief Get the list of video tracks
     * @return The list of video tracks. If no video track was found, it returns an empty list
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    QList<VideoTrack> videoTracks() const;

    /**
     * @brief Get the list of subtitles tracks
     * @return The list of subtitles tracks. If no subtitles track was found, it returns an empty list
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    QList<Track> subtitlesTracks() const;

    /**
     * @brief Get audio tracks name
     * @return A list that contains a description for each audio tracks
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    QStringList audioTracksName() const;

    /**
     * @brief Get video tracks name
     * @return A list that contains a description for each video tracks
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    QStringList videoTracksName() const;

    /**
     * @brief Get subtitles tracks name
     * @return A list that contains a description for each subtitles tracks
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    QStringList subtitlesTracksName() const;

    /**
     * @brief Media is an image
     * @return True if media file is an image, false otherwise
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    bool isImage() const;

    /**
     * @brief Add `count` to tell the media is used
     * @param count The number to add
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void usageCountAdd(int count = 1);

    /**
     * @brief Media is used or not
     * @return True if the media is used, false otherwise.
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    bool isUsed() const;

    /**
     * @brief Get list of availabled audio extensions
     * @return The list of audio extensions
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    static QStringList audioExtensions();

    /**
     * @brief Get list of availabled video extensions
     * @return The list of video extensions
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    static QStringList videoExtensions();

    /**
     * @brief Get list of availabled image extensions
     * @return The list of image extensions
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    static QStringList imageExtensions();

    /**
     * @brief Get list of availabled media extensions (audio, video and image)
     * @return The list of media extensions
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    static QStringList mediaExtensions();

    /**
     * @brief Method used to set the time for images
     *
     * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
     */
    void setImageTime(QString time);

    /**
     * @brief Method used to set the duration of media
     *
     * @author Denis Saunier <saunier.denis.86@gmail.com>
     */
    void setDuration(QString &time);

    /**
     * @brief Method used to get the original duration of media
     *
     * @author Denis Saunier <saunier.denis.86@gmail.com>
     */
    uint getOriginalDuration();

    /**
     * @brief Method used to get the full path of the media
     *
     * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
     */
    QString getLocation();
protected:

    /**
     * @brief Parse media informations with libvlc. It is used to fetch tracks informations and duration.
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void parseMediaInfos();

signals:
    /**
     * @brief emitted when usage count changed (less or more)
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void usageCountChanged();

private:
    /**
     * @brief The number of created instances, used to set a unique identifier for each instance.
     */
    static int s_instanceCount;

    /**
     * @brief The instance identifier. Used for data serialization
     */
    int _id;

    /**
     * @brief The current file location
     */
    QString _location;

    /**
     * @brief Helper, it is used to fetch file informations
     */
    QFileInfo _fileInfo;

    /**
     * @brief The libvlc media core
     */
    libvlc_media_t *_vlcMedia;

    /**
     * @brief The list of audio tracks
     */
    QList<AudioTrack> _audioTracks;

    /**
     * @brief The list of video tracks
     */
    QList<VideoTrack> _videoTracks;

    /**
     * @brief The list of subtitles tracks
     */
    QList<Track> _subtitlesTracks;

    /**
     * @brief If the value is 0, it means the media is not used.
     */
    int _usageCount;

    /**
     * @brief Duration of media.
     */
    uint _duration;

    /**
     * @brief Original duration of media.
     */
    uint _originalDuration;
};

#endif // MEDIA_H
