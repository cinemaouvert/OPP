/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
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

#ifndef SEEKWIDGET_H_
#define SEEKWIDGET_H_

#include <QApplication>
#include <QPoint>
#include <QWidget>
#include <QPushButton>
#include <QSlider>

#include "PlaylistPlayer.h"
#include "PlayerControlWidget.h"
#include "FlipBar.h"

class QLabel;
class QProgressBar;
class QTimer;
class MediaPlayer;
class FlipBar;

class SeekWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SeekWidget(QWidget *parent = 0);

    ~SeekWidget();

    /**
     * @brief Returns the value of autoHide
     * @return The value of autoHide
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    bool autoHide() const { return _autoHide; }

    /**
     * @brief Set the value of autoHide
     * @param autoHide The new value of autoHide
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setAutoHide(const bool &autoHide);

    /**
     * @brief setPlaylistPlayer
     * @param player
     */
    void setPlaylistPlayer(PlaylistPlayer *player);

    /**
     * @brief get the flip bar
     */
    FlipBar* flipBar(){ return _seek; }

protected:

    /**
     * @brief wheelEvent
     * @param event
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void wheelEvent(QWheelEvent *event);

private slots:
    /**
     * @brief Reset the widget
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void end();

    /**
     * @brief Update the elapsed time value and the remaining time value.
     * @param time The new elapsed time value
     */
    void updateTime(const int &time);

    /**
     * @brief Update the seek widget length value
     * @param time The new full time value
     */
    void updateLength(const int &time);

private:
    /**
     * @brief Initialize the widget
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void initSeekWidget();

    /**
     * @brief connection
     */
    void connection();

    /**
     * @brief disconnection
     */
    void disconnection();


    /**
     * @brief _playlistPlayer The playlist player
     */
    PlaylistPlayer *_playlistPlayer;

    /**
     * @brief _mediaPlayer The media player
     */
    MediaPlayer *_mediaPlayer;

    /**
     * @brief _autoHide
     */
    bool _autoHide;

    /**
     * @brief _seek The flip bar
     */
    FlipBar *_seek;

    /**
     * @brief _labelElapsed Label for elapsed time
     */
    QLabel *_labelElapsed;

    /**
     * @brief _labelFull Label for total time
     */
    QLabel *_labelRemaining;

};

#endif // SEEKWIDGET_H_
