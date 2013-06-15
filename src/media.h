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

class Media : public QObject
{
    Q_OBJECT
public:
    Media(const QString &location, libvlc_instance_t *vlcInstance, QObject *parent = 0);
    Media(const Media &media);
    virtual ~Media();

    /**
     * @brief initMedia
     * @param location
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void initMedia(const QString &location);

    /**
     * @brief operator =
     * @param media
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    Media & operator=(const Media &media);

    /**
     * @brief operator ==
     * @param media
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    bool operator==(const Media &media) const;

    /**
     * @brief setId
     * @param id
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setId(int id);

    /**
     * @brief location
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline const QString & location() const { return _location; }

    /**
     * @brief core
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline libvlc_media_t* core() const { return _vlcMedia; }

    /**
     * @brief id
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline int id() const { return _id; }

    /**
     * @brief duration
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    uint duration() const;

    /**
     * @brief name
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    QString name() const;

    /**
     * @brief exists
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    bool exists() const;

    /**
     * @brief audioTracks
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    QList<AudioTrack> audioTracks() const;

    /**
     * @brief videoTracks
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    QList<VideoTrack> videoTracks() const;

    /**
     * @brief subtitlesTracks
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    QList<Track> subtitlesTracks() const;

    /**
     * @brief audioTracksName
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    QStringList audioTracksName() const;

    /**
     * @brief videoTracksName
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    QStringList videoTracksName() const;

    /**
     * @brief subtitlesTracksName
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    QStringList subtitlesTracksName() const;

    /**
     * @brief isImage
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    bool isImage() const;

    /**
     * @brief usageCountAdd
     * @param count
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void usageCountAdd(int count = 1);

    /**
     * @brief isUsed
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    bool isUsed() const;

    /**
     * @brief audioExtensions
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    static QStringList audioExtensions();

    /**
     * @brief videoExtensions
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    static QStringList videoExtensions();

    /**
     * @brief imageExtensions
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    static QStringList imageExtensions();

    /**
     * @brief mediaExtensions
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    static QStringList mediaExtensions();

protected:
    /**
     * @brief parseMediaInfos
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void parseMediaInfos();

signals:
    /**
     * @brief usageCountChanged
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void usageCountChanged();

private:
    /**
     * @brief s_instanceCount
     */
    static int s_instanceCount;

    /**
     * @brief _id
     */
    int _id;

    /**
     * @brief _location
     */
    QString _location;

    /**
     * @brief _fileInfo
     */
    QFileInfo _fileInfo;

    /**
     * @brief _vlcMedia
     */
    libvlc_media_t *_vlcMedia;

    /**
     * @brief _audioTracks
     */
    QList<AudioTrack> _audioTracks;

    /**
     * @brief _videoTracks
     */
    QList<VideoTrack> _videoTracks;

    /**
     * @brief _subtitlesTracks
     */
    QList<Track> _subtitlesTracks;

    /**
     * @brief _usageCount
     */
    int _usageCount;
};

#endif // MEDIA_H
