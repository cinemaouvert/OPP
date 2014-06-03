/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
 *
 * Open Projection Program is an initiative of Catalogue Ouvert du Cinéma.
 * The software was developed by four students of University of Poitiers
 * as school project.
 *
 * Open Projection Program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Open Projection Program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Open Projection Program. If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************************/

#include "mediasettings.h"

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

void MediaSettings::setSubtitlesEncode(const int &encode){
    _subtitlesEncode = encode;
     emit subtitlesEncodeChanged(_subtitlesEncode);
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


void MediaSettings::setCrop(int top,int left, int right, int bot){
    _cropTop = top;
    _cropBot = bot;
    _cropRight = right;
    _cropLeft = left;

    emit cropChanged(_cropTop,_cropLeft,_cropRight,_cropBot);
}

void MediaSettings::setAudioFadeOut(int time)
{
    _audioFadeOut = time;
    emit audioFadeOutChanged(_audioFadeOut);
}

void MediaSettings::setGain(float gain)
{
    _gain=gain;
    emit gainChanged(_gain);
}

void MediaSettings::initDefault()
{
    _audioTrack = 0; /* audio disabled */
    _videoTrack = 0; /* video disabled */
    _subtitlesTrack = 0; /* subtitles disabled */
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
    _subtitlesEncode = 0;
    _cropTop = 0;
    _cropBot = 0;
    _cropLeft = 0;
    _cropRight = 0;
    _audioFadeOut = 0;
    _audioFadeIn = 0;
    _videoFadeOut = 0;
    _videoFadeIn = 0;
}

QStringList MediaSettings::ratioValues()
{
    QStringList values;
    values << tr("Default")
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

QStringList MediaSettings::encodeValues()
{
    QStringList encodes;
    encodes <<   ""
           <<   "UTF-8"
           <<   "UTF-16"
           <<   "UTF-16BE"
           <<   "UTF-16LE"
           <<   "GB18030"
           <<   "ISO-8859-15"
           <<   "Windows-1252"
           <<   "IBM850"
           <<   "ISO-8859-2"
           <<   "Windows-1250"
           <<   "ISO-8859-3"
           <<   "ISO-8859-10"
           <<   "Windows-1251"
           <<   "KOI8-R"
           <<   "KOI8-U"
           <<   "ISO-8859-6"
           <<   "Windows-1256"
           <<   "ISO-8859-7"
           <<   "Windows-1253"
           <<   "ISO-8859-8"
           <<   "Windows-1255"
           <<   "ISO-8859-9"
           <<   "Windows-1254"
           <<   "ISO-8859-11"
           <<   "Windows-874"
           <<   "ISO-8859-13"
           <<   "Windows-1257"
           <<   "ISO-8859-14"
           <<   "ISO-8859-16"
           <<   "ISO-2022-CN-EXT"
           <<   "EUC-CN"
           <<   "ISO-2022-JP-2"
           <<   "EUC-JP"
           <<   "Shift_JIS"
           <<   "CP949"
           <<   "ISO-2022-KR"
           <<   "Big5"
           <<   "ISO-2022-TW"
           <<   "Big5-HKSCS"
           <<   "VISCII"
           <<   "Windows-1258";

    return encodes;
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
