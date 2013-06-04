#include "mediasettings.h"

MediaSettings::MediaSettings(QObject *parent) :
    QObject(parent)
{
    initDefault();
}

MediaSettings::MediaSettings(MediaSettings &mediasettings)
{

}

void MediaSettings::initDefault()
{
    _ratio = Original;
    _scale = NoScale;
    _deinterlacing = Disabled;
    _subtitlesSync = 0;
    _gamma = 50;
    _contrast = 50;
    _brightness = 50;
    _saturation = 50;
    _hue = 0;
}

MediaSettings& MediaSettings::operator =(const MediaSettings &mediasettings)
{
    if (this != &mediasettings) {

    }
    return *this;
}

QStringList MediaSettings::ratioValues()
{
    QStringList values;
    values << ""
        << "16:9"
        << "16:10"
        << "185:100"
        << "221:100"
        << "235:100"
        << "239:100"
        << "4:3"
        << "5:4"
        << "5:3"
        << "1:1";

    return values;
}

QStringList MediaSettings::deinterlacingValues()
{
    QStringList list;
    list << ""
        << "discard"
        << "blend"
        << "man"
        << "bob"
        << "linear"
        << "x";

    return list;
}

QList<float> MediaSettings::scaleValues()
{
    QList<float> list;
    list << (float) 0
        << (float) 1.05
        << (float) 1.1
        << (float) 1.2
        << (float) 1.3
        << (float) 1.4
        << (float) 1.5
        << (float) 1.6
        << (float) 1.7
        << (float) 1.8
        << (float) 1.9
        << (float) 2.0;

    return list;
}
