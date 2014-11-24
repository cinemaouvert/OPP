/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
 *          Geoffrey Bergé <geoffrey.berge@live.fr>
 *          Thibaud Lamarche <lamarchethibaud@hotmail.com>
 *          Denis Saunier <saunier.denis.86@gmail.com>
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

#ifndef MEDIATABLEVIEW_H
#define MEDIATABLEVIEW_H

#include <QTableView>

class MediaTableView : public QTableView
{
    Q_OBJECT
public:
    explicit MediaTableView(QWidget *parent = 0);

    void setModel(QAbstractItemModel *model);
    
public slots:
    void showHeaders(int mediaCount);

protected:
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
     * @brief dragLeaveEvent
     * @param event
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void dragLeaveEvent(QDragLeaveEvent *event);

private:
    /**
     * @brief _startPos Mouse press position
     */
    QPoint startPos;
    
};

#endif // MEDIATABLEVIEW_H
