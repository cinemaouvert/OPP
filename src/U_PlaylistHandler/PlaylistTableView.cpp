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

#include "PlaylistTableView.h"
#include "playlistmodel.h"
#include <QMouseEvent>
#include <QApplication>
#include <QItemSelectionModel>
#include <QDebug>
#include <QMenu>
#include <QHeaderView>
#include <QFileDialog>
#include <QSettings>
#include <QUrl>

#if (QT_VERSION >= 0x050000) // Qt version 5 and above
    #include <QDrag>
#endif

#include "mainwindow.h"
#include "media.h"
#include "playback.h"


PlaylistTableView::PlaylistTableView(MainWindow* mainWindow, PlaylistTabWidget* playlistTabWidget, int tabIndex, QWidget *parent) :
    QTableView(parent),
    _mainWindow(mainWindow),
    _tabIndex(tabIndex),
    _scS(NULL)
{
    setAcceptDrops(true);
    setDragEnabled(true);
    setSelectionMode(QTableView::SingleSelection);
    setDragDropMode(QTableView::DropOnly);
    horizontalHeader()->setVisible(false);
    verticalHeader()->setVisible(false);
    _scS = new ScreenshotSelector(_mainWindow);

    #if (QT_VERSION >= 0x050000) // Qt version 5 and above
        verticalHeader()->setSectionsMovable(true);
    #else // until version 5
        verticalHeader()->setMovable(true);
    #endif

    setDropIndicatorShown(true);

    _playlistTabWidget = playlistTabWidget;

    connect(_playlistTabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
    connect(_playlistTabWidget, SIGNAL(currentChanged(int)), _mainWindow->playlistHandlerWidget()->playlistDetailsWidget(), SLOT(updatePlaylistWidget()));
    connect(mainWindow->playerControlWidget(), SIGNAL(stopState()), this, SLOT(toggleIconParentTab()));
    connect(mainWindow->playlistPlayer(), SIGNAL(itemChanged(int)), this, SLOT(selectRow(int)));

    setStyleSheet(QString("font-size: 13px;"));
    setAlternatingRowColors(true);
}

PlaylistTableView::~PlaylistTableView(){
    if(_scS != NULL)
        delete _scS;
}

void PlaylistTableView::setModel(PlaylistModel *model)
{
    QTableView::setModel(model);

    connect(model, SIGNAL(layoutChanged()), this, SLOT(selectPlayingItem()));
    connect(model, SIGNAL(layoutChanged()), this, SLOT(showHeaders()));
    connect(model, SIGNAL(layoutChanged()), this, SLOT(toggleIconParentTab()));
}

void PlaylistTableView::selectPlayingItem()
{
    PlaylistModel* playlistModel = (PlaylistModel*)model();

    if(playlistModel->isRunning()){
        selectRow(playlistModel->activeItemIndex());
    }
}

void PlaylistTableView::toggleIconParentTab()
{
    PlaylistModel* playlistModel = (PlaylistModel*)model();
    if(playlistModel != NULL)
    {
        if(playlistModel->state() == PlaylistModel::Playing)
        {
            _mainWindow->playlistTabWidget()->iconPlayAt(_tabIndex);
        }
        else if(playlistModel->state() == PlaylistModel::Paused)
        {
            _mainWindow->playlistTabWidget()->iconPauseAt(_tabIndex);
        }
        else if(playlistModel->state() == PlaylistModel::Idle)
        {
            if(selectionModel()->selectedRows().count() > 0){
                _mainWindow->playlistTabWidget()->iconStopAt(_tabIndex);
            }else{
                _mainWindow->playlistTabWidget()->removeIconAt(_tabIndex);
            }
        }
    }
}

void PlaylistTableView::mousePressEvent(QMouseEvent *event)
{
    QTableView::mousePressEvent(event);

    if (event->button() == Qt::LeftButton) {
        _startPos = event->pos();
    }else{
        QModelIndexList indexes = selectionModel()->selectedRows();
        if (indexes.count() > 0)
        {
            QPoint globalPos = this->mapToGlobal(event->pos());

            QModelIndex indexes = _mainWindow->currentPlaylistTableView()->currentIndex();
            Media *media = _mainWindow->currentPlaylistModel()->playlist()->at(indexes.row())->media();

            QMenu contextMenu;
            QAction *change = new QAction(tr("Change screenshot"), this);
            QAction *advanced = new QAction(tr("Advanced settings"), this);
            QAction *advancedPic = new QAction(tr("Advanced picture settings"), this);
            QAction *addSubtitleFile = new QAction(tr("Add subtitle file"), this);

            contextMenu.addAction(change);
            contextMenu.addAction(advanced);
            contextMenu.addAction(advancedPic);
            contextMenu.addAction(addSubtitleFile);
            change->setEnabled(false);
            if(!media->isAudio() && !media->isImage()){
                change->setEnabled(true);
            }

            QAction* selectedItem = contextMenu.exec(globalPos);
            if (selectedItem == change)
            {
                if(!media->isAudio() && !media->isImage()){
                    _scS->setMedia(media);
                    _scS->show();
                }
            }
            else if(selectedItem == advanced)
            {
                _mainWindow->openAdvancedSettingsWindow();
            }
            else if(selectedItem == advancedPic)
            {
                _mainWindow->openAdvancedPictureSettings();
            }
            else if(selectedItem == addSubtitleFile)
            {
               QSettings settings("opp", "opp");
               QString fileName = QFileDialog::getOpenFileName(this, tr("Add subtitle"), settings.value("moviesPath").toString(), tr("File (*.srt *.ass)"));
               //On ajoute ça pour windows pour que ce soit le vrai chemin avec les "\".
             const QString nativePath = QDir::toNativeSeparators(fileName);

             if(!fileName.isEmpty()){

               ((Playback*)((PlaylistModel*)model())->playlist()->at(indexes.row()))->mediaSettings()->setSubtitlesFile( nativePath.toStdString().c_str());
               ((PlaylistModel*)model())->setData(_mainWindow->currentPlaylistModel()->index(indexes.row(), 5), nativePath, Qt::DisplayRole);
               _mainWindow->add_Item_SubtitleTrackComboBox(nativePath.toStdString().c_str());
               ((Playback*)((PlaylistModel*)model())->playlist()->at(indexes.row()))->mediaSettings()->setSubtitlesTrack(1);

               //((Playback*)((PlaylistModel*)model())->playlist()->at(indexes.row()))->media()->subtitlesTracks() << nativePath;
               ((Playback*)((PlaylistModel*)model())->playlist()->at(indexes.row()))->media()->addSubtitleTrackName(nativePath);

               //_mainWindow->subtitleSave(nativePath)
               int index = ((Playback*)((PlaylistModel*)model())->playlist()->at(indexes.row()))->media()->subtitlesTracksName().count();
               _mainWindow->set_SubtitleTrackComboBox_index(0);
               _mainWindow->set_SubtitleTrackComboBox_index(index);
             }
            }
        }
    }

}

void PlaylistTableView::mouseDoubleClickEvent ( QMouseEvent * event ){
    int distance = (event->pos() - _startPos).manhattanLength();
    if (distance < QApplication::startDragDistance()){
        QModelIndexList indexes = selectionModel()->selectedRows();
        if (indexes.count() > 0)
            _mainWindow->openAdvancedSettingsWindow();
    }
}

void PlaylistTableView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() && Qt::LeftButton)
    {
        int distance = (event->pos() - _startPos).manhattanLength();
        if (distance >= QApplication::startDragDistance())
            startDrag(Qt::MoveAction);
    }
    QTableView::mouseMoveEvent(event);
}

void PlaylistTableView::startDrag(Qt::DropActions supportedActions)
{
    Q_UNUSED(supportedActions);

    QModelIndexList indexes = selectionModel()->selectedRows();

    if (indexes.count() == 0)
        return;


    QModelIndex index = indexes.first();

    QMimeData *mimedata = new QMimeData;

    mimedata->setText(index.data().toString());
    mimedata->setHtml("#"+QString::number(index.row()));

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimedata);

    drag->start(Qt::CopyAction | Qt::MoveAction);
}

void PlaylistTableView::dragEnterEvent(QDragEnterEvent *event)
{
    PlaylistTableView *source = (PlaylistTableView *)(event->source());

    /**
     * Internal move
     */
    if (source)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();

    /**
     * Comes from outside
     */
    }else{
        // Vérifie s'il s'agit d'une video
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void PlaylistTableView::dragMoveEvent(QDragMoveEvent *event)
{
    PlaylistTableView *source = (PlaylistTableView *)(event->source());
    if (source) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void PlaylistTableView::dropEvent(QDropEvent *event)
{
    /**
     * Internal move
     */
    PlaylistTableView *source = (PlaylistTableView *)(event->source());
    if (source) {
        model()->dropMimeData(event->mimeData(), event->dropAction(), indexAt(event->pos()).row(), 0, indexAt(event->pos()));
        event->acceptProposedAction();
    }
    /**
     * from outside
     */
    else{
        // Add the media to the current playlist
        event->acceptProposedAction();

        // Add the media to the bin
        foreach(QUrl url, event->mimeData()->urls()){
            QString urlStr = url.toString();

            // Remove the "file://" prefix
            QString file = urlStr.mid(7);

            int index = _mainWindow->addMedia(file);

            Playback* playback = new Playback(_mainWindow->mediaListModel()->mediaList().at(index));
            ((PlaylistModel*)model())->addPlayback(playback);
        }
    }

    QTableView::dropEvent(event);
}

void PlaylistTableView::selectionChanged (const QItemSelection & selected, const QItemSelection & deselected){
    PlaylistPlayer* playlistPlayer = _mainWindow->getPlaylistPlayer();

    if(playlistPlayer->mediaPlayer()->isStopped()){
        if(selected.count() > 0)
        {
            _mainWindow->playlistTabWidget()->iconStopAt(_tabIndex);

            /** initialize the selected item to playback */
            playlistPlayer->initItemAt(selected.first().top());
            _mainWindow->setSelectedMediaTimeByIndex(selected.first().top());
        }
        else
        {
            _mainWindow->playlistTabWidget()->removeIconAt(_tabIndex);

            _mainWindow->setSelectedMediaNameByIndex(-1);
            _mainWindow->setSelectedMediaTimeByIndex(-1);
        }
    }
    QAbstractItemView::selectionChanged(selected, deselected);
}

void PlaylistTableView::tabChanged(int currentIndex){
    Q_UNUSED(currentIndex);

    if(!shown()){
        // Remove all selections
        clearSelection();

    }else{
        // Remove all selections
        clearSelection();

        // Select the playing item
        selectPlayingItem();
    }
}

bool PlaylistTableView::shown(){
    return (PlaylistTableView*)_playlistTabWidget->currentWidget() == this;
}

void PlaylistTableView::clearSelection(){
    if(selectionModel() != NULL){
        selectionModel()->clear();
    }

    // Remove tab icon
    PlaylistModel* playlistModel = (PlaylistModel*)model();
    if(playlistModel != NULL){
        if(playlistModel->state() == PlaylistModel::Idle)
            _mainWindow->playlistTabWidget()->removeIconAt(_tabIndex);
    }
}

void PlaylistTableView::showHeaders(){
    if(model()->rowCount() > 0){
        horizontalHeader()->setVisible(true);
        verticalHeader()->setVisible(true);
    }else{
        horizontalHeader()->setVisible(false);
        verticalHeader()->setVisible(false);
    }
}
