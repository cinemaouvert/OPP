#include "mediasettings.h"
#include "QDebug"

MediaSettings::MediaSettings(QObject *parent) :
    QObject(parent)
{
    initDefault();
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

void MediaSettings::setAudioTrack(const int &track) {
    _audioTrack = track;
    emit audioTrackChanged(_audioTrack);
}

void MediaSettings::setVideoTrack(const int &track){
    _videoTrack = track;
    emit videoTrackChanged(_videoTrack);
}

void MediaSettings::setSubtitlesTrack(const int &track){
    _subtitlesTrack = track;
    emit subtitlesTrackChanged(_subtitlesTrack);
}

void MediaSettings::setGamma(float gamma) {
    _gamma = gamma;
    emit gammaChanged(_gamma);
}

void MediaSettings::setContrast(float contrast) {
    _contrast = contrast;
    emit contrastChanged(_contrast);
}

void MediaSettings::setBrightness(float brightness) {
    _brightness = brightness;
    emit brightnessChanged(_brightness);
}

void MediaSettings::setSaturation(float saturation) {
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

void MediaSettings::setTestPattern(bool testpattern)
{
    _testPattern = testpattern;
    emit testPatternChanged(_testPattern);
}

void MediaSettings::setInMark(int mark)
{
    _inMark = mark;
    emit inMarkChanged(_inMark);
}

void MediaSettings::setOutMark(int mark)
{
    _outMark = mark;
    emit outMarkChanged(_outMark);
}

void MediaSettings::setGain(float gain)
{
    _gain=gain;
    emit gainChanged(_gain);
}

void MediaSettings::initDefault()
{
    _audioTrack = 0;
    _videoTrack = 0;
    _subtitlesTrack = 0;
    _ratio = Original;
    _scale = NoScale;
    _deinterlacing = Discard;
    _subtitlesSync = 0;
    _gamma = 1;
    _contrast = 1;
    _brightness = 1;
    _saturation = 1;
    _hue = 0;
    _audioSync=0;
    _testPattern = false;
    _inMark = 0;
    _outMark = 0;
    _gain=0;
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
    list << "Discard"
        << "Blend"
        << "Mean"
        << "Bob"
        << "Linear";

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
