/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *          Thibaud Lamarche <lamarchethibaud@hotmail.com>
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
#include "videowidget.h"

VideoWindow::VideoWindow(QWidget *parent, const DisplayMode &mode) :
    QMainWindow(parent)
{
    _videoWidget = new VideoWidget(this);
    setCentralWidget(_videoWidget);

    setWindowTitle("Video");
    resize(640,480);

    setDisplayMode(mode);

    _f1_shortcut = new QShortcut(QKeySequence("f1"), this);
    _escape_shortcut = new QShortcut(QKeySequence(tr("escape")), this);

    connect(_f1_shortcut, SIGNAL(activated()), this, SLOT(switchVideoMode()));
    connect(_escape_shortcut, SIGNAL(activated()), this, SLOT(escapeFullscreen()));
}

VideoWindow::VideoWindow(QWidget *parent, int width, int height) :
    QMainWindow(parent)
{
    _videoWidget = new VideoWidget(this);
    setCentralWidget(_videoWidget);

    setWindowTitle("Video");
    resize(width, height);
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
        setCursor(Qt::BlankCursor);
        break;
    case WINDOW:
        moveToDisplay(0);
        showNormal();
        setCursor(Qt::ArrowCursor);
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
    Q_UNUSED(event);
    emit(closed());
}

void VideoWindow::switchVideoMode(){
    ((MainWindow*)parent())->switchVideoMode();
}

void VideoWindow::escapeFullscreen()
{
    if(isFullScreen()){
        if(!((MainWindow*)parent())->locker()->isLock()){
            ((MainWindow*)parent())->switchVideoMode();
        }
    }
}
