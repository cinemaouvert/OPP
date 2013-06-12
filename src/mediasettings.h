#ifndef MEDIASETTINGS_H
#define MEDIASETTINGS_H

#include <QObject>
#include <QStringList>
#include <QList>
#include <QDebug>

#include "track.h"
#include "videotrack.h"
#include "audiotrack.h"

enum Ratio {
    Original = 0,
    R_16_9 = 1,
    R_16_10 = 2,
    R_185_100 = 3,
    R_221_100 = 4,
    R_235_100 = 5,
    R_239_100 = 6,
    R_4_3 = 7,
    R_5_4 = 8,
    R_5_3 = 9,
    R_1_1 = 10
};

enum Scale {
    NoScale = 0,
    S_1_05 = 1,
    S_1_1 = 2,
    S_1_2 = 3,
    S_1_3 = 4,
    S_1_4 = 5,
    S_1_5 = 6,
    S_1_6 = 7,
    S_1_7 = 8,
    S_1_8 = 9,
    S_1_9 = 10,
    S_2_0 = 11
};

enum Deinterlacing {
    Disabled = 0,
    Discard = 1,
    Blend = 2,
    Mean = 3,
    Bob = 4,
    Linear = 5,
    X = 6
};

class MediaSettings : public QObject
{
    Q_OBJECT
public:
    explicit MediaSettings(QObject *parent = 0);
    MediaSettings(MediaSettings &mediasettings);
    MediaSettings& operator =(const MediaSettings &mediasettings);
    
    void initDefault();

    inline Ratio ratio() const { return _ratio; }
    inline Scale scale() const { return _scale; }
    inline Deinterlacing deinterlacing() const { return _deinterlacing; }
    inline double subtitlesSync() const { return _subtitlesSync; }
    inline float gamma() const { return _gamma; }
    inline float contrast() const { return _contrast; }
    inline float brightness() const { return _brightness; }
    inline float saturation() const { return _saturation; }
    inline int hue() const { return _hue; }
    inline double audioSync() const { return _audioSync; }
    inline const AudioTrack & audioTrack() const { return _audioTrack; }
    inline const VideoTrack & videoTrack() const { return _videoTrack; }
    inline const Track & subtitlesTrack() const { return _subtitlesTrack; }

    void setRatio(Ratio ratio);
    void setScale(Scale scale);
    void setDeinterlacing(Deinterlacing deinterlacing);
    void setSubtitlesSync(double sync);
    void setGamma(float gamma);
    void setContrast(float contrast);
    void setBrightness(float brightness);
    void setSaturation(float saturation);
    void setHue(int hue);
    void setAudioSync(double sync);
    void setAudioTrack(const AudioTrack &track);
    void setVideoTrack(const VideoTrack &track);
    void setSubtitlesTrack(const Track &track);

    static QStringList ratioValues();
    static QStringList deinterlacingValues();
    static QList<float> scaleValues();


signals:
    void ratioChanged(Ratio);
    void scaleChanged(Scale);
    void deinterlacingChanged(Deinterlacing);
    void subtitlesSyncChanged(double);
    void gammaChanged(float);
    void contrastChanged(float);
    void brightnessChanged(float);
    void saturationChanged(float);
    void hueChanged(int);
    void audioSyncChanged(double sync);
    void audioTrackChanged(AudioTrack);
    void videoTrackChanged(VideoTrack);
    void subtitlesTrackChanged(Track);

private:
    Ratio _ratio;
    Scale _scale;
    Deinterlacing _deinterlacing;
    double _subtitlesSync;
    float _gamma;
    float _contrast;
    float _brightness;
    float _saturation;
    int _hue;
    double _audioSync;

    AudioTrack _audioTrack;
    VideoTrack _videoTrack;
    Track _subtitlesTrack;
};

#endif // MEDIASETTINGS_H
