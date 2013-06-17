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

#include <QPoint>
#include <QWidget>

class QLabel;
class QProgressBar;
class QTimer;

class MediaPlayer;

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
     * @brief Set a media player
     * @param player The new media player
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setMediaPlayer(MediaPlayer *player);

protected:
    /**
     * @brief mouseMoveEvent
     * @param event
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void mouseMoveEvent(QMouseEvent *event);

    /**
     * @brief mousePressEvent
     * @param event
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void mousePressEvent(QMouseEvent *event);

    /**
     * @brief mouseReleaseEvent
     * @param event
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void mouseReleaseEvent(QMouseEvent *event);

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
     * @brief Update the elapsed time value
     * @param time The new elapsed time value
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void updateCurrentTime(const int &time);

    /**
     * @brief Update the full time value
     * @param time The new full time value
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void updateFullTime(const int &time);

private:
    /**
     * @brief Initialize the widget
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void initSeekWidget();

    /**
     * @brief Update the widget
     * @param pos
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void updateEvent(const QPoint &pos);

    /**
     * @brief Lock the widget
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void lock();

    /**
     * @brief Unlock the widget
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void unlock();

    /**
     * @brief _lock Indicates if the widget is locked
     */
    bool _lock;

    /**
     * @brief _vlcMediaPlayer The media player
     */
    MediaPlayer *_vlcMediaPlayer;

    /**
     * @brief _autoHide
     */
    bool _autoHide;
    /**
     * @brief _seek The progress bar
     */
    QProgressBar *_seek;
    /**
     * @brief _labelElapsed Label for elapsed time
     */
    QLabel *_labelElapsed;
    /**
     * @brief _labelFull Label for total time
     */
    QLabel *_labelFull;
};

#endif // SEEKWIDGET_H_
