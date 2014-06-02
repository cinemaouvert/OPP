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

#include "mediatableview.h"
#include "medialistmodel.h"
#include <QMouseEvent>
#include <QApplication>
#include <QItemSelectionModel>
#include <QDebug>

MediaTableView::MediaTableView(QWidget *parent) :
    QTableView(parent)
{
    setDragDropMode(QAbstractItemView::DragOnly);
    setSelectionMode(QTableView::ContiguousSelection);
}

void MediaTableView::mousePressEvent(QMouseEvent *event)
{
    QTableView::mousePressEvent(event);
}

void MediaTableView::mouseReleaseEvent(QMouseEvent *event){

    QTableView::mouseReleaseEvent(event);
}

void MediaTableView::dragLeaveEvent(QDragLeaveEvent *event){
    QTableView::dragLeaveEvent(event);
}



void MediaTableView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        int distance = (event->pos() - startPos).manhattanLength();
        if (distance >= QApplication::startDragDistance()+50) {
            startDrag(Qt::MoveAction);
            selectionModel()->clearSelection();
        }
    }
}

void MediaTableView::startDrag(Qt::DropActions supportedActions)
{
    QModelIndexList indexes = selectionModel()->selectedRows();

    if (indexes.count() == 0)
        return;

    QString paths;
    QModelIndex index;

    foreach (index, indexes) {
        paths += QString::number(index.row());
        paths += ":";
    }

    QMimeData *mimedata = new QMimeData;
    mimedata->setText("drop your media to the playlist");
    mimedata->setHtml(paths.toUtf8());

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimedata);

    if (drag->start(Qt::CopyAction | Qt::MoveAction) == Qt::MoveAction) {
        selectionModel()->clearSelection();
    }
}

void MediaTableView::dragEnterEvent(QDragEnterEvent *event)
{
    MediaTableView *source = (MediaTableView *)(event->source());

    if (source) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    QTableView::dragEnterEvent(event);
}

void MediaTableView::dragMoveEvent(QDragMoveEvent *event)
{
    MediaTableView *source = (MediaTableView *)(event->source());

    if (source) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    QTableView::dragMoveEvent(event);
}

void MediaTableView::dropEvent(QDropEvent *event)
{
    QTableView::dropEvent(event);
}
