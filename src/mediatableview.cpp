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

#include "mediatableview.h"
#include "medialistmodel.h"
#include <QMouseEvent>
#include <QApplication>
#include <QItemSelectionModel>
#include <QDebug>
#include <QHeaderView>
#include <QStandardItem>
#if (QT_VERSION >= 0x050000) // Qt version 5 and above
    #include <QMimeData>
    #include <QDrag>
#endif

MediaTableView::MediaTableView(QWidget *parent) :
    QTableView(parent)
{
    setDragDropMode(QAbstractItemView::DragOnly);
    setSelectionMode(QTableView::ContiguousSelection);
    verticalHeader()->setVisible(false);
    horizontalHeader()->setVisible(false);
}

void MediaTableView::setModel(QAbstractItemModel *model)
{
    if(this->model() != NULL){
        disconnect(((MediaListModel*)this->model()), SIGNAL(mediaListChanged(int)), this, SLOT(showHeaders(int)));
    }
    connect(((MediaListModel*)model), SIGNAL(mediaListChanged(int)), this, SLOT(showHeaders(int)));
    QTableView::setModel(model);
    /*for(int i=0; i<((MediaListModel*)model)->indexfichiermanquant().count()+1; i++ )
    {
        int index = ((MediaListModel*)model)->indexfichiermanquant().at(i);
        QTableView::selectRow(index);
        QModelIndexList indexes = selectionModel()->selectedRows();
        QStandardItem item(index);
        item.setData(QColor(Qt::green), Qt::BackgroundRole);
        item.setData(QColor(Qt::red), Qt::FontRole);
    }*/
}

void MediaTableView::mousePressEvent(QMouseEvent *event)
{
    QTableView::mousePressEvent(event);
    startPos = event->pos();
}

void MediaTableView::mouseReleaseEvent(QMouseEvent *event){

    QTableView::mouseReleaseEvent(event);
    startPos.setX(0);
    startPos.setY(0);
}

void MediaTableView::dragLeaveEvent(QDragLeaveEvent *event){
    QTableView::dragLeaveEvent(event);
}

void MediaTableView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        int distance = (event->pos() - startPos).manhattanLength();
        if (distance >= QApplication::startDragDistance()+25) {
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

void MediaTableView::showHeaders(int mediaCount)
{
    if(mediaCount <= 0){
        horizontalHeader()->setVisible(false);
        verticalHeader()->setVisible(false);
    }else{
        horizontalHeader()->setVisible(true);
        verticalHeader()->setVisible(true);
    }
}
