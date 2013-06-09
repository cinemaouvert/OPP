#ifndef MEDIASETTINGS_H
#define MEDIASETTINGS_H

#include <QObject>
#include <QStringList>
#include <QList>
#include <QDebug>

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
    inline int gamma() const { return _gamma; }
    inline int contrast() const { return _contrast; }
    inline int brightness() const { return _brightness; }
    inline int saturation() const { return _saturation; }
    inline int hue() const { return _hue; }
    inline double audioSync() const { return _audioSync; }
    inline int audioTrack() const {return _audioTrack;}
    inline int videoTrack() const {return _videoTrack;}
    inline int subtitlesTrack() const {return _subtitlesTrack;}

    void setRatio(Ratio ratio);
    void setScale(Scale scale);
    void setDeinterlacing(Deinterlacing deinterlacing);
    void setSubtitlesSync(double sync);
    void setGamma(int gamma);
    void setContrast(int contrast);
    void setBrightness(int brightness);
    void setSaturation(int saturation);
    void setHue(int hue);
    void setAudioSync(double sync);
    void setAudioTrack(int track);
    void setVideoTrack(int track);
    void setSubtitlesTrack(int track);

    static QStringList ratioValues();
    static QStringList deinterlacingValues();
    static QList<float> scaleValues();


signals:
    void ratioChanged(Ratio);
    void scaleChanged(Scale);
    void deinterlacingChanged(Deinterlacing);
    void subtitlesSyncChanged(double);
    void gammaChanged(int);
    void contrastChanged(int);
    void brightnessChanged(int);
    void saturationChanged(int);
    void hueChanged(int);
    void audioSyncChanged(double sync);
    void audioTrackChanged(int track);
    void videoTrackChanged(int track);
    void subtitlesTrackChanged(int track);


private:
    Ratio _ratio;
    Scale _scale;
    Deinterlacing _deinterlacing;
    double _subtitlesSync;
    int _gamma;
    int _contrast;
    int _brightness;
    int _saturation;
    int _hue;
    double _audioSync;

    int _audioTrack;
    int _videoTrack;
    int _subtitlesTrack;
};

#endif // MEDIASETTINGS_H
