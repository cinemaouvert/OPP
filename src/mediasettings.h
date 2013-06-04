#ifndef MEDIASETTINGS_H
#define MEDIASETTINGS_H

#include <QObject>
#include <QStringList>
#include <QList>

enum Ratio {
    Original,
    R_16_9,
    R_16_10,
    R_185_100,
    R_221_100,
    R_235_100,
    R_239_100,
    R_4_3,
    R_5_4,
    R_5_3,
    R_1_1
};

enum Scale {
    NoScale,
    S_1_05,
    S_1_1,
    S_1_2,
    S_1_3,
    S_1_4,
    S_1_5,
    S_1_6,
    S_1_7,
    S_1_8,
    S_1_9,
    S_2_0
};

enum Deinterlacing {
    Disabled,
    Discard,
    Blend,
    Mean,
    Bob,
    Linear,
    X
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

    static QStringList ratioValues();
    static QStringList deinterlacingValues();
    static QList<float> scaleValues();

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
};

#endif // MEDIASETTINGS_H
