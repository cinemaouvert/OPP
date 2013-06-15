#ifndef MEDIASETTINGS_H
#define MEDIASETTINGS_H

#include <QObject>
#include <QStringList>
#include <QList>
#include <QDebug>

#include "track.h"
#include "videotrack.h"
#include "audiotrack.h"

/**
 * @enum Ratio
 * @brief The Ratio enum
 */
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

/**
 * @enum Scale
 * @brief The Scale enum
 */
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

/**
 * @enum Deinterlacing
 * @brief The Deinterlacing enum
 */
enum Deinterlacing {
    Discard = 0,
    Blend = 1,
    Mean = 2,
    Bob = 3,
    Linear = 4
};

/**
 * @class MediaSettings
 * @brief The MediaSettings class
 */
class MediaSettings : public QObject
{
    Q_OBJECT
public:
    explicit MediaSettings(QObject *parent = 0);

    /**
     * @brief initDefault
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void initDefault();

    /**
     * @brief ratio
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline Ratio ratio() const { return _ratio; }

    /**
     * @brief scale
     * @return
     */
    inline Scale scale() const { return _scale; }

    /**
     * @brief deinterlacing
     * @return
     */
    inline Deinterlacing deinterlacing() const { return _deinterlacing; }

    /**
     * @brief subtitlesSync
     * @return
     */
    inline double subtitlesSync() const { return _subtitlesSync; }

    /**
     * @brief gamma
     * @return
     */
    inline float gamma() const { return _gamma; }

    /**
     * @brief contrast
     * @return
     */
    inline float contrast() const { return _contrast; }

    /**
     * @brief brightness
     * @return
     */
    inline float brightness() const { return _brightness; }

    /**
     * @brief saturation
     * @return
     */
    inline float saturation() const { return _saturation; }

    /**
     * @brief hue
     * @return
     */
    inline int hue() const { return _hue; }

    /**
     * @brief audioSync
     * @return
     */
    inline double audioSync() const { return _audioSync; }

    /**
     * @brief audioTrack
     * @return
     */
    inline const int & audioTrack() const { return _audioTrack; }

    /**
     * @brief videoTrack
     * @return
     */
    inline const int & videoTrack() const { return _videoTrack; }

    /**
     * @brief subtitlesTrack
     * @return
     */
    inline const int & subtitlesTrack() const { return _subtitlesTrack; }

    /**
     * @brief testPattern
     * @return
     */
    inline bool testPattern() const {return _testPattern;}

    /**
     * @brief inMark
     * @return
     */
    inline int inMark() const {return _inMark;}

    /**
     * @brief outMark
     * @return
     */
    inline int outMark() const {return _outMark;}

    /**
     * @brief gain
     * @return
     */
    inline float gain() const {return _gain;}

    /**
     * @brief setRatio
     * @param ratio
     */
    void setRatio(Ratio ratio);

    /**
     * @brief setScale
     * @param scale
     */
    void setScale(Scale scale);

    /**
     * @brief setDeinterlacing
     * @param deinterlacing
     */
    void setDeinterlacing(Deinterlacing deinterlacing);

    /**
     * @brief setSubtitlesSync
     * @param sync
     */
    void setSubtitlesSync(double sync);

    /**
     * @brief setGamma
     * @param gamma
     */
    void setGamma(float gamma);

    /**
     * @brief setContrast
     * @param contrast
     */
    void setContrast(float contrast);

    /**
     * @brief setBrightness
     * @param brightness
     */
    void setBrightness(float brightness);

    /**
     * @brief setSaturation
     * @param saturation
     */
    void setSaturation(float saturation);

    /**
     * @brief setHue
     * @param hue
     */
    void setHue(int hue);

    /**
     * @brief setAudioSync
     * @param sync
     */
    void setAudioSync(double sync);

    /**
     * @brief setGain
     * @param gain
     */
    void setGain(float gain);

    /**
     * @brief setAudioTrack
     * @param track
     */
    void setAudioTrack(const int &track);

    /**
     * @brief setVideoTrack
     * @param track
     */
    void setVideoTrack(const int &track);

    /**
     * @brief setSubtitlesTrack
     * @param track
     */
    void setSubtitlesTrack(const int &track);

    /**
     * @brief setTestPattern
     * @param testpattern
     */
    void setTestPattern(bool testpattern);

    /**
     * @brief setInMark
     * @param mark
     */
    void setInMark (int mark);

    /**
     * @brief setOutMark
     * @param mark
     */
    void setOutMark(int mark);

    /**
     * @brief ratioValues
     * @return
     */
    static QStringList ratioValues();

    /**
     * @brief deinterlacingValues
     * @return
     */
    static QStringList deinterlacingValues();

    /**
     * @brief scaleValues
     * @return
     */
    static QList<float> scaleValues();

signals:

    /**
     * @brief ratioChanged
     */
    void ratioChanged(Ratio);

    /**
     * @brief scaleChanged
     */
    void scaleChanged(Scale);

    /**
     * @brief deinterlacingChanged
     */
    void deinterlacingChanged(Deinterlacing);

    /**
     * @brief subtitlesSyncChanged
     */
    void subtitlesSyncChanged(double);

    /**
     * @brief gammaChanged
     */
    void gammaChanged(float);

    /**
     * @brief contrastChanged
     */
    void contrastChanged(float);

    /**
     * @brief brightnessChanged
     */
    void brightnessChanged(float);

    /**
     * @brief saturationChanged
     */
    void saturationChanged(float);

    /**
     * @brief hueChanged
     */
    void hueChanged(int);

    /**
     * @brief audioSyncChanged
     * @param sync
     */
    void audioSyncChanged(double sync);

    /**
     * @brief audioTrackChanged
     */
    void audioTrackChanged(int);

    /**
     * @brief videoTrackChanged
     */
    void videoTrackChanged(int);

    /**
     * @brief subtitlesTrackChanged
     */
    void subtitlesTrackChanged(int);

    /**
     * @brief testPatternChanged
     */
    void testPatternChanged(bool);

    /**
     * @brief inMarkChanged
     */
    void inMarkChanged(int);

    /**
     * @brief outMarkChanged
     */
    void outMarkChanged(int);

    /**
     * @brief gainChanged
     */
    void gainChanged(float);

private:

    /**
     * @brief _ratio
     */
    Ratio _ratio;

    /**
     * @brief _scale
     */
    Scale _scale;

    /**
     * @brief _deinterlacing
     */
    Deinterlacing _deinterlacing;

    /**
     * @brief _subtitlesSync
     */
    double _subtitlesSync;

    /**
     * @brief _gamma
     */
    float _gamma;

    /**
     * @brief _contrast
     */
    float _contrast;

    /**
     * @brief _brightness
     */
    float _brightness;

    /**
     * @brief _saturation
     */
    float _saturation;

    /**
     * @brief _hue
     */
    int _hue;

    /**
     * @brief _audioSync
     */
    double _audioSync;

    /**
     * @brief _audioTrack
     */
    int _audioTrack;

    /**
     * @brief _videoTrack
     */
    int _videoTrack;

    /**
     * @brief _subtitlesTrack
     */
    int _subtitlesTrack;

    /**
     * @brief _testPattern
     */
    bool _testPattern;

    /**
     * @brief _inMark
     */
    int _inMark;

    /**
     * @brief _outMark
     */
    int _outMark;

    /**
     * @brief _gain
     */
    float _gain;
};

#endif // MEDIASETTINGS_H
