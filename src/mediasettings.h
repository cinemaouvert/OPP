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
 * @brief Contains available video ratio values
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
 * @brief Contains available video scale values
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
 * @brief Contains available deinterlacing modes
 */
enum Deinterlacing {
    Discard = 0,
    Blend = 1,
    Mean = 2,
    Bob = 3,
    Linear = 4
};

/**
 * @brief Manage media settings
 */
class MediaSettings : public QObject
{
    Q_OBJECT
public:
    explicit MediaSettings(QObject *parent = 0);

    /**
     * @brief initialize default settings values
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void initDefault();

    /**
     * @brief Get video ratio
     * @return The video ratio
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline Ratio ratio() const { return _ratio; }

    /**
     * @brief Get video scale
     * @return The video scale
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline Scale scale() const { return _scale; }

    /**
     * @brief Get video deinterlacing
     * @return The video ratio
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline Deinterlacing deinterlacing() const { return _deinterlacing; }

    /**
     * @brief Get subtitles synchronization
     * @return The subtitles synchronization
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    inline double subtitlesSync() const { return _subtitlesSync; }

    /**
     * @brief Get current gamma value
     * @return The gamme value
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    inline float gamma() const { return _gamma; }

    /**
     * @brief Get current contrast value
     * @return The contrast value
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    inline float contrast() const { return _contrast; }

    /**
     * @brief Get current brightness value
     * @return The brightness value
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    inline float brightness() const { return _brightness; }

    /**
     * @brief Get current saturation value
     * @return The current saturation value
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    inline float saturation() const { return _saturation; }

    /**
     * @brief Get current hue value
     * @return The hue value
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    inline int hue() const { return _hue; }

    /**
     * @brief Get current audio synchronization value
     * @return The audio synchronization value
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    inline double audioSync() const { return _audioSync; }

    /**
     * @brief Get current audio track value
     * @return The audio track value
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline const int & audioTrack() const { return _audioTrack; }

    /**
     * @brief Get current video track value
     * @return The video track value
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline const int & videoTrack() const { return _videoTrack; }

    /**
     * @brief Get current subtitles track value
     * @return The subtitles track value
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline const int & subtitlesTrack() const { return _subtitlesTrack; }

    /**
     * @brief Is test pattern enabled
     * @return True if test pattern is enabled, false otherwise
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    inline bool testPattern() const {return _testPattern;}

    /**
     * @brief Get in mark position
     * @return The in mark position in ms
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    inline int inMark() const {return _inMark;}

    /**
     * @brief Get out mark position
     * @return The out mark position in ms
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    inline int outMark() const {return _outMark;}

    /**
     * @brief Get current audio gain
     * @return The audio gain
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    inline float gain() const {return _gain;}

    /**
     * @brief setRatio
     * @param ratio
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setRatio(Ratio ratio);

    /**
     * @brief setScale
     * @param scale
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setScale(Scale scale);

    /**
     * @brief setDeinterlacing
     * @param deinterlacing
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setDeinterlacing(Deinterlacing deinterlacing);

    /**
     * @brief setSubtitlesSync
     * @param sync
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void setSubtitlesSync(double sync);

    /**
     * @brief setGamma
     * @param gamma
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void setGamma(float gamma);

    /**
     * @brief setContrast
     * @param contrast
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void setContrast(float contrast);

    /**
     * @brief setBrightness
     * @param brightness
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void setBrightness(float brightness);

    /**
     * @brief setSaturation
     * @param saturation
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void setSaturation(float saturation);

    /**
     * @brief setHue
     * @param hue
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void setHue(int hue);

    /**
     * @brief setAudioSync
     * @param sync
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void setAudioSync(double sync);

    /**
     * @brief setGain
     * @param gain
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void setGain(float gain);

    /**
     * @brief setAudioTrack
     * @param track
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setAudioTrack(const int &track);

    /**
     * @brief setVideoTrack
     * @param track
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setVideoTrack(const int &track);

    /**
     * @brief setSubtitlesTrack
     * @param track
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setSubtitlesTrack(const int &track);

    /**
     * @brief setTestPattern
     * @param testpattern
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void setTestPattern(bool testpattern);

    /**
     * @brief setInMark
     * @param mark
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void setInMark (int mark);

    /**
     * @brief setOutMark
     * @param mark
     *
     * @author Cyril Naud <futuramath@gmail.com>
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
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    static QStringList deinterlacingValues();

    /**
     * @brief scaleValues
     * @return
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
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
