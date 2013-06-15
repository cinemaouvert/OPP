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
    Discard = 0,
    Blend = 1,
    Mean = 2,
    Bob = 3,
    Linear = 4
};

class MediaSettings : public QObject
{
    Q_OBJECT
public:
    explicit MediaSettings(QObject *parent = 0);

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
    inline const int & audioTrack() const { return _audioTrack; }
    inline const int & videoTrack() const { return _videoTrack; }
    inline const int & subtitlesTrack() const { return _subtitlesTrack; }
    inline bool testPattern() const {return _testPattern;}
    inline int inMark() const {return _inMark;}
    inline int outMark() const {return _outMark;}
    inline float gain() const {return _gain;}

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
    void setGain(float gain);

    void setAudioTrack(const int &track);
    void setVideoTrack(const int &track);
    void setSubtitlesTrack(const int &track);

    void setTestPattern(bool testpattern);
    void setInMark (int mark);
    void setOutMark(int mark);

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

    void audioTrackChanged(int);
    void videoTrackChanged(int);
    void subtitlesTrackChanged(int);

    void testPatternChanged(bool);
    void inMarkChanged(int);
    void outMarkChanged(int);

    void gainChanged(float);

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

    int _audioTrack;
    int _videoTrack;
    int _subtitlesTrack;

    bool _testPattern;
    int _inMark;
    int _outMark;

    float _gain;
};

#endif // MEDIASETTINGS_H
