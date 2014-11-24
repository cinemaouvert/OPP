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

#include "VLCApplication.h"

#include <QStringList>
#include <QSettings>
#include <QDebug>

#include <vlc/vlc.h>

VLCApplication::VLCApplication()
{
    QSettings settings("opp", "opp");
    QStringList vlcargs;
        vlcargs << "--intf=dummy"
                << "--no-media-library"

    #if !defined(Q_OS_MAC)
                << "--no-one-instance" // Allow multiple starts
    #endif
                << "--ignore-config" // Don't use VLC's config files
                << "--no-plugins-cache"
                << "--no-stats" // We don't want statistics
                << "--no-osd"
                << "--no-loop"
                << "--no-video-title-show" // N'incruste pas le titre de la video dans la video
    #if defined(Q_OS_MAC)
                << "--vout=macosx"
    #endif
                << "--drop-late-frames"
                << "--no-snapshot-preview";

    if(settings.contains("subtitleColor")){
        vlcargs << "--sub-filter=freetype"
                << "--freetype-color=" + subtitleColor(settings.value("subtitleColor").toInt());
    }

    initVlcInstanceFromArgs(vlcargs);
}

VLCApplication::~VLCApplication()
{
    this->closeLibvlc();
}

void VLCApplication::initVlcInstanceFromArgs(const QStringList &args)
{
    char **argV = (char **)malloc(sizeof(char **) * args.count());
    for (int i = 0; i < args.count(); ++i)
        argV[i] = (char *)qstrdup(args.at(i).toLocal8Bit().data());

    // Create new libvlc instance
    _vlcInstance = libvlc_new(args.count(), argV);

    if (_vlcInstance) {
        qDebug() << "Using libvlc version:" << QString(libvlc_get_version());
    } else {
        qDebug() << "OPP error: libvlc failed to load!";
        this->closeLibvlc();
        abort();
    }
}

void VLCApplication::closeLibvlc()
{
    libvlc_release(_vlcInstance);
}

QString VLCApplication::subtitleColor(int index){
    QStringList sl;
    sl <<"16777215" << "16776960";
    return sl.at(index);
}
