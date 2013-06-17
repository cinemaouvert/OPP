/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
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

    bool autoHide() const { return _autoHide; }

    void setAutoHide(const bool &autoHide);

    void setMediaPlayer(MediaPlayer *player);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private slots:
    void end();
    void updateCurrentTime(const int &time);
    void updateFullTime(const int &time);

private:
    void initSeekWidget();
    void updateEvent(const QPoint &pos);

    void lock();
    void unlock();

    bool _lock;

    MediaPlayer *_vlcMediaPlayer;

    bool _autoHide;
    QProgressBar *_seek;
    QLabel *_labelElapsed;
    QLabel *_labelFull;
};

#endif // SEEKWIDGET_H_
