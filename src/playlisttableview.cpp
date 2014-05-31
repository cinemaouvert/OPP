/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Cyril Naud <futuramath@gmail.com>
 *          Florian Mhun <florian.mhun@gmail.com>
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

#include "playlisttableview.h"
#include "playlistmodel.h"
#include <QMouseEvent>
#include <QApplication>
#include <QItemSelectionModel>
#include <QDebug>
#include <QMenu>
#include "mainwindow.h"
#include "media.h"

PlaylistTableView::PlaylistTableView(MainWindow* mainWindow,QWidget *parent) :
    QTableView(parent),
    _mainWindow(mainWindow),
    _scS(NULL)
{
    setAcceptDrops(true);
    setSelectionMode(QTableView::SingleSelection);
    _scS = new ScreenshotSelector(_mainWindow);

}

PlaylistTableView::~PlaylistTableView(){
    if(_scS != NULL)
        delete _scS;
}


void PlaylistTableView::mousePressEvent(QMouseEvent *event)
{
    QTableView::mousePressEvent(event);

    if (event->button() == Qt::LeftButton) {
        startPos = event->pos();
    }else{
        QModelIndexList indexes = selectionModel()->selectedRows();
        if (indexes.count() > 0)
        {
            QPoint globalPos = this->mapToGlobal(event->pos());

            QModelIndex indexes = _mainWindow->currentPlaylistTableView()->currentIndex();
            Media *media = _mainWindow->currentPlaylistModel()->playlist()->at(indexes.row())->media();

            QMenu myMenu;
            QAction *change = new QAction(tr("Change screnshot"), this);
            QAction *advanced = new QAction(tr("Advanced settings"), this);
            QAction *advancedPic = new QAction(tr("Advanced picture settings"), this);

            myMenu.addAction(change);
            myMenu.addAction(advanced);
            myMenu.addAction(advancedPic);
            change->setEnabled(false);
            if(!media->isAudio() && !media->isImage()){
                change->setEnabled(true);
            }

            QAction* selectedItem = myMenu.exec(globalPos);
            if (selectedItem == change)
            {
                if(!media->isAudio() && !media->isImage()){
                    _scS->setMedia(media);
                    _scS->show();
                }
            }
            else if(selectedItem == advanced)
            {
                _mainWindow->on_advancedSettingsButton_clicked();
            }
            else if(selectedItem == advancedPic)
            {
                _mainWindow->on_advancedPictureSettingsButton_clicked();
            }
        }
    }

}

void PlaylistTableView::mouseDoubleClickEvent ( QMouseEvent * event ){
    int distance = (event->pos() - startPos).manhattanLength();
    if (distance < QApplication::startDragDistance()){
        QModelIndexList indexes = selectionModel()->selectedRows();
        if (indexes.count() > 0)
            _mainWindow->on_advancedSettingsButton_clicked();
    }
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

void PlaylistTableView::selectionChanged (const QItemSelection & selected, const QItemSelection & deselected){
    if(selected.count() > 0)
    {
        _mainWindow->setSelectedMediaNameByIndex(selected.first().top());
        if(!_mainWindow->getPlaylistPlayer()->isPlaying())
        {
            _mainWindow->setSelectedMediaTimeByIndex(selected.first().top());
        }
    }
    else
    {
        _mainWindow->setSelectedMediaNameByIndex(-1);
        _mainWindow->setSelectedMediaTimeByIndex(-1);
        this->selectRow(this->currentIndex().row());
    }
    QAbstractItemView::selectionChanged (selected,deselected);

}

