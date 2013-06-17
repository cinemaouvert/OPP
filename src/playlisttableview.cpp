/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Cyril Naud <futuramath@gmail.com>
 *          Florian Mhun <florian.mhun@gmail.com>
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

#include "playlisttableview.h"
#include "playlistmodel.h"
#include <QMouseEvent>
#include <QApplication>
#include <QItemSelectionModel>
#include <QDebug>

PlaylistTableView::PlaylistTableView(QWidget *parent) :
    QTableView(parent)
{
    setAcceptDrops(true);
}

void PlaylistTableView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        startPos = event->pos();
    }
    QTableView::mousePressEvent(event);
}

void PlaylistTableView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() && Qt::LeftButton)
    {
        int distance = (event->pos() - startPos).manhattanLength();
        if (distance >= QApplication::startDragDistance())
            startDrag(Qt::MoveAction);
    }
    QTableView::mouseMoveEvent(event);
}

void PlaylistTableView::startDrag(Qt::DropActions supportedActions)
{
    QModelIndexList indexes = selectionModel()->selectedRows();

    if (indexes.count() == 0)
        return;

    QModelIndex index = indexes.first();

    QMimeData *mimedata = new QMimeData;

    mimedata->setText(index.data().toString());

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimedata);

    if (drag->start(Qt::CopyAction | Qt::MoveAction) == Qt::MoveAction) {
        clearSelection();
    }
}

void PlaylistTableView::dragEnterEvent(QDragEnterEvent *event)
{
    PlaylistTableView *source = (PlaylistTableView *)(event->source());
    if (source && source != this)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void PlaylistTableView::dragMoveEvent(QDragMoveEvent *event)
{
    PlaylistTableView *source = (PlaylistTableView *)(event->source());
    if (source && source != this) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void PlaylistTableView::dropEvent(QDropEvent *event)
{
    model()->dropMimeData(event->mimeData(), event->dropAction(), 0, 0, indexAt(event->pos()));
    event->acceptProposedAction();

    QTableView::dropEvent(event);
}
