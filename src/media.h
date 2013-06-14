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

    void initMedia(const QString &location);

    Media & operator=(const Media &media);
    bool operator==(const Media &media) const;

    inline const QString & location() const { return _location; }
    inline libvlc_media_t* core() const { return _vlcMedia; }
    inline QSize videoResolution() const { return _videoResolution; }

    uint duration() const;
    QString name() const;

    bool exists() const;

    QList<AudioTrack> audioTracks() const;
    QList<VideoTrack> videoTracks() const;
    QList<Track> subtitlesTracks() const;

    QStringList audioTracksName() const;
    QStringList videoTracksName() const;
    QStringList subtitlesTracksName() const;

    bool isImage() const;

    void usageCountAdd(int count = 1);
    bool isUsed() const;

    static QStringList audioExtensions();
    static QStringList videoExtensions();
    static QStringList imageExtensions();
    static QStringList mediaExtensions();

protected:
    void parseMediaInfos();

signals:
    void usageCountChanged();

private:
    QString _location;
    QFileInfo _fileInfo;

    libvlc_media_t *_vlcMedia;

    QList<AudioTrack> _audioTracks;
    QList<VideoTrack> _videoTracks;
    QList<Track> _subtitlesTracks;

    int _usageCount;
    QSize _videoResolution;

    friend QDataStream & operator << (QDataStream &, const QList<Media> &);
    friend QDataStream & operator >> (QDataStream &, QList<Media> &);
};

#endif // MEDIA_H
