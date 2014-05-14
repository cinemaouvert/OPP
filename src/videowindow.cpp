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

#include "videowindow.h"

#include <QApplication>
#include <QDesktopWidget>

#include <iostream>
#include "mainwindow.h"
#include "playlistplayer.h"
#include "videowidget.h"

VideoWindow::VideoWindow(QWidget *parent ,const DisplayMode &mode) :
    QMainWindow(parent)
{
    _videoWidget = new VideoWidget(this);
    setCentralWidget(_videoWidget);

    setWindowTitle("Video");
    resize(640,480);

    setDisplayMode(mode);
}

VideoWindow::~VideoWindow()
{
    delete _videoWidget;
}

void VideoWindow::setDisplayMode(const DisplayMode &mode)
{
    _mode = mode;

    switch (_mode) {
    case PROJECTION:
        moveToDisplay(1);
        showFullScreen();
        break;
    case WINDOW:
        moveToDisplay(0);
        showNormal();
        break;
    }
}

void VideoWindow::moveToDisplay(const int &display)
{
    QRect secondDisplayRes = QApplication::desktop()->screenGeometry(display);

    move(QPoint(secondDisplayRes.x(), secondDisplayRes.y()));
}

void VideoWindow::closeEvent (QCloseEvent *event)
{
    ((MainWindow * )this->parentWidget())->stop();
}
