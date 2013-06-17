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

#ifndef VIDEOWIDGET_H_
#define VIDEOWIDGET_H_

#include <QTimer>
#include <QWidget>
#include <QLayout>

#if defined(Q_OS_MAC)
    #include <QMacCocoaViewContainer>
#endif

#include "videoview.h"

#if defined(Q_OS_MAC)
class VideoWidget : public QMacCocoaViewContainer, public VideoView
#else
class VideoWidget : public QWidget, public VideoView
#endif
{
Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent = 0);

    ~VideoWidget();

    /**
     * @overload
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    WId request();

    /**
     * @overload
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void release();

private:
    /**
     * @brief Initialize video widget
     */
    void init();

    /**
     * @brief sync
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void sync();

#if !defined(Q_OS_MAC)
    QWidget *_video;
    QLayout *_layout;
#endif
};

#endif // VIDEOWIDGET_H_
