#include "mediasettings.h"
#include "QDebug"

MediaSettings::MediaSettings(QObject *parent) :
    QObject(parent)
{
    initDefault();
}

MediaSettings::MediaSettings(MediaSettings &mediasettings)
{
    _ratio = mediasettings.ratio();
}

MediaSettings& MediaSettings::operator =(const MediaSettings &mediasettings)
{
    if (this != &mediasettings) {
        _ratio = mediasettings.ratio();
    }
    return *this;
}

void MediaSettings::setRatio(Ratio ratio) {
    _ratio = ratio;
    emit ratioChanged(_ratio);
}

void MediaSettings::setScale(Scale scale) {
    _scale = scale;
    emit scaleChanged(_scale);
}

void MediaSettings::setDeinterlacing(Deinterlacing deinterlacing){
    _deinterlacing = deinterlacing;
    emit deinterlacingChanged(_deinterlacing);
}

void MediaSettings::setSubtitlesSync(double sync){
    _subtitlesSync = sync;
    emit subtitlesSyncChanged(_subtitlesSync);
}

void MediaSettings::setAudioTrack(int track){
    _audioTrack=track;
    emit audioTrackChanged(_audioTrack);
}

void MediaSettings::setVideoTrack(int track){
    _videoTrack=track;
    emit videoTrackChanged(_videoTrack);
}

void MediaSettings::setSubtitlesTrack(int track){
    _subtitlesTrack=track;
    emit subtitlesTrackChanged(_subtitlesTrack);
}

void MediaSettings::setGamma(int gamma) {
    _gamma = gamma;
    emit gammaChanged(_gamma);
}

void MediaSettings::setContrast(int contrast) {
    _contrast = contrast;
    emit contrastChanged(_contrast);
}

void MediaSettings::setBrightness(int brightness) {
    _brightness = brightness;
    emit brightnessChanged(_brightness);
}

void MediaSettings::setSaturation(int saturation) {
    _saturation = saturation;
    emit saturationChanged(_saturation);
}

void MediaSettings::setHue(int hue) {
    _saturation = hue;
    emit saturationChanged(_saturation);
}

void MediaSettings::setAudioSync(double sync){
    _audioSync = sync;
    emit audioSyncChanged(_audioSync);
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
    _audioTrack=2;
    _videoTrack=0;
    _subtitlesTrack=4;
    _audioSync=0;
}

QStringList MediaSettings::ratioValues()
{
    QStringList values;
    values << "default"
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
    list << "default"
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
