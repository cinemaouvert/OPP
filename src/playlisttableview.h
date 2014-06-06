/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Cyril Naud <futuramath@gmail.com>
 *          Florian Mhun <florian.mhun@gmail.com>
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

#ifndef PLAYLISTTABLEVIEW_H
#define PLAYLISTTABLEVIEW_H

#include <QTableView>
#include "schedulelistmodel.h"
#include "mainwindow.h"
#include "screenshotselector.h"


class PlaylistTableView : public QTableView
{
    Q_OBJECT
public:
    explicit PlaylistTableView(MainWindow* mainWindow,QWidget *parent = 0);
    virtual ~PlaylistTableView();

protected:
    /**
     * @brief mousePressEvent
     * @param event
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void mousePressEvent(QMouseEvent *event);

    /**
     * @brief mouseDoubleClickEvent
     * @param event
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void mouseDoubleClickEvent(QMouseEvent *event);

    /**
     * @brief mouseMoveEvent
     * @param event
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void mouseMoveEvent(QMouseEvent *event);

    /**
     * @brief dragEnterEvent
     * @param event
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void dragEnterEvent(QDragEnterEvent *event);

    /**
     * @brief dragMoveEvent
     * @param event
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void dragMoveEvent(QDragMoveEvent *event);

    /**
     * @brief dropEvent
     * @param event
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void dropEvent(QDropEvent *event);

    /**
     * @brief startDrag
     * @param event
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void startDrag(Qt::DropActions supportedActions);

    /**
     * @brief selectionChanged
     * @param event
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void selectionChanged (const QItemSelection & selected, const QItemSelection & deselected);

private:
    /**
     * @brief _startPos Mouse press position
     */
    QPoint startPos;

    /**
     * @brief _scheduleListModel List model of schedules
     */
    ScheduleListModel *_scheduleListModel;

    /**
     * @brief _mainWindow The Main Window
     */
    MainWindow *_mainWindow;

    /**
     * @brief _scS The screenshot selector
     */
    ScreenshotSelector *_scS;
};

#endif // PLAYLISTTABLEVIEW_H
