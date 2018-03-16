#include "PlaylistTabWidget.h"
#include "playlistmodel.h"
#include "playlisttableview.h"
#include "locker.h"

#include <QInputDialog>
#include <QHeaderView>

PlaylistTabWidget::PlaylistTabWidget(MainWindow* mw, QWidget* parent):
    QTabWidget(parent),
    _activeTab(-1)
{
    _mw = mw;
    _playlistPlayer = _mw->playlistPlayer();
    _lastSelectedTab = -1;

    #if defined(Q_OS_LINUX)
        setStyleSheet(QString::fromUtf8(
            "QTableView{"
            "   border-top: 1px solid rgb(180, 180, 180);"
            "}"
        ));
     #endif

    setFocusPolicy(Qt::NoFocus);
    setTabsClosable(true);
    setMovable(true);
    setIconSize(QSize(10, 10));

    MediaPlayer* mediaPlayer = _playlistPlayer->mediaPlayer();
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(tabCloseRequested(int)));
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(currentChanged(int)));
    connect(mediaPlayer, SIGNAL(stopped()), this, SLOT(connectionCurrentTab()));
}

void PlaylistTabWidget::tabCloseRequested(int index)
{
    Playlist *playlist = ((PlaylistHandlerWidget*) parent())->playlistAt(index);

    PlaylistModel *model = (PlaylistModel*) ((PlaylistTableView*) widget(index))->model();
    if(model->isRunning()){
        QMessageBox::critical(this, tr("Remove playlist"), tr("This playlist is currently running, you can't delete it.") , tr("OK"));
    }else{
        if (_mw->scheduleListModel()->isScheduled(playlist)) {
            if (0 == QMessageBox::warning(this, tr("Remove playlist"), tr("This playlist was scheduled. All schedules which use this playlist will be deleted too.\n Are you sure to remove this playlist ?") ,tr("No"), tr("Yes")))
                return;
            _mw->scheduleListModel()->removeScheduleWithDeps(playlist);
        }

        if (0 == QMessageBox::warning(this, tr("Remove playlist"), tr("Are you sure to remove this playlist ?") ,tr("No"), tr("Yes"))){
            return;
        }

        if (count() == 1) {
            createTab("playlist");
        }else{
            ((PlaylistTableView*) widget(index))->deleteLater();
        }

        removeTab(index);


        _mw->updateSettings();
        _mw->updatePlaylistListCombox();
        _mw->updateDetails();
        _mw->updateProjectSummary();
    }
}

void PlaylistTabWidget::currentChanged(int index)
{
    Q_UNUSED(index);

    if(_lastSelectedTab != -1){
        PlaylistTableView *view = (PlaylistTableView*) widget(_lastSelectedTab);
        if(view != NULL){
            PlaylistModel *model = (PlaylistModel*) view->model();
            if(model != NULL){
                if(model->isRunning()){
                    return;
                }
                deconnection();
            }
        }
    }

    connection();
}

void PlaylistTabWidget::createTab(QString name)
{
    if(name == "playlist"){
        name = tr("New Playlist");
    }

    if(_mw->locker()->isLock())
        QMessageBox::critical(this, tr("Add new playlist"), tr("The playlist is currently locked, you can not add a new playlist.") , tr("OK"));
    else {
        PlaylistTableView *newTab = new PlaylistTableView(_mw, this, count() - 1);
        Playlist *playlist = new Playlist(name);
        PlaylistModel *newModel = new PlaylistModel(playlist, _mw->mediaListModel(), _mw->scheduleListModel(), _mw);

        connect(playlist, SIGNAL(titleChanged()), _mw->scheduleListModel(), SIGNAL(layoutChanged()));
        connect(playlist, SIGNAL(playlistChanged()), _mw, SLOT(updateDetails()));
        connect(playlist, SIGNAL(titleChanged()), _mw->playlistHandlerWidget()->playlistDetailsWidget(), SLOT(updatePlaylistWidget()));
        connect(playlist, SIGNAL(playlistChanged()), _mw->playlistHandlerWidget()->playlistDetailsWidget(), SLOT(updatePlaylistWidget()));

        newTab->setModel(newModel);
        newTab->setSelectionBehavior(QAbstractItemView::SelectRows);
        newTab->horizontalHeader()->setStretchLastSection(true);

        // Resize the first column
        newTab->setColumnWidth(0, 40);

        addTab(newTab, playlist->title());
        setCurrentWidget(newTab);

        //Add the empty icon
        removeIconAt(count() - 1);

        _mw->updatePlaylistListCombox();
    }
}

void PlaylistTabWidget::restoreTab(PlaylistModel *model)
{
    PlaylistTableView *newTab = new PlaylistTableView(_mw, this, count() - 1);
    connect(model->playlist(), SIGNAL(titleChanged()), _mw->scheduleListModel(), SIGNAL(layoutChanged()));

    newTab->setModel(model);
    newTab->setSelectionBehavior(QAbstractItemView::SelectRows);
    newTab->horizontalHeader()->setStretchLastSection(true);

    // Resize the first column
    newTab->setColumnWidth(0, 40);

    addTab(newTab, model->playlist()->title());
    setCurrentWidget(newTab);

    // add the empty icon
    removeIconAt(count());

    _mw->updatePlaylistListCombox();
}

void PlaylistTabWidget::deconnection(){
    if(_lastSelectedTab != -1 && _lastSelectedTab != currentIndex()){
        PlaylistTableView *view = (PlaylistTableView*) widget(_lastSelectedTab);
        if(view != NULL){
            PlaylistModel *model = (PlaylistModel*) view->model();
            if(model != NULL){
                if(model->isRunning()){
                    model->stopItem();
                }
                disconnect(view, SIGNAL(clicked(QModelIndex)), _mw, SLOT(updateSettings()));

                disconnect(_mw->playerControlWidget(), SIGNAL(playState()), model, SLOT(playItem()));
                disconnect(_mw->playerControlWidget(), SIGNAL(pauseState()), model, SLOT(pauseItem()));
                disconnect(_mw->playerControlWidget(), SIGNAL(stopState()), model, SLOT(stopItem()));
                disconnect(_playlistPlayer, SIGNAL(end()), model, SLOT(stopItem()));
                disconnect(_playlistPlayer, SIGNAL(itemChanged(int)), model, SLOT(setActiveItem(int)));
            }
        }
    }
}

void PlaylistTabWidget::connection(){
    if(_lastSelectedTab != currentIndex()){
        PlaylistTableView *view = (PlaylistTableView*) currentWidget();
        PlaylistModel *model = (PlaylistModel*) view->model();

        // Connection
        connect(view, SIGNAL(clicked(QModelIndex)), _mw, SLOT(updateSettings()));

        connect(_mw->playerControlWidget(), SIGNAL(playState()), model, SLOT(playItem()));
        connect(_mw->playerControlWidget(), SIGNAL(pauseState()), model, SLOT(pauseItem()));
        connect(_mw->playerControlWidget(), SIGNAL(stopState()), model, SLOT(stopItem()));
        connect(_playlistPlayer, SIGNAL(end()), model, SLOT(stopItem()));
        connect(_playlistPlayer, SIGNAL(itemChanged(int)), model, SLOT(setActiveItem(int)));

        _mw->updateSettings();

        _playlistPlayer->setPlaylist(model->playlist());

        _lastSelectedTab = currentIndex();
    }
}

void PlaylistTabWidget::connectionCurrentTab(){
    deconnection();
    connection();
}

void PlaylistTabWidget::editPlaylistName()
{
    if(_mw->locker()->isLock())
        QMessageBox::critical(_mw, tr("Edit playlist name"), tr("This playlist is currently locked, you can not edit the name of the playlist.") , tr("OK"));
    else {
        int tabIndex = currentIndex();
        bool ok;

        QString text = QInputDialog::getText(_mw,
                                             tr("Rename playlist"),
                                             tr("Playlist title : "),
                                             QLineEdit::Normal,
                                             tabText(tabIndex),
                                             &ok
                                             );

        if (ok && !text.isEmpty()) {
            setTabText(tabIndex, text);
            ((PlaylistModel*)((PlaylistTableView*) currentWidget())->model())->playlist()->setTitle(text);
            _mw->updatePlaylistListCombox();
        }
    }
}

void PlaylistTabWidget::iconPlayAt(int index)
{
    if(_activeTab == -1)
        _activeTab = index;

    if(_activeTab != index){
        removeIconAt(_activeTab);
        _activeTab = index;
    }

    QIcon icon;
    icon.addFile(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_173_play.png"), QSize(), QIcon::Normal, QIcon::Off);
    setTabIcon(index, icon);
}

void PlaylistTabWidget::iconPauseAt(int index)
{
    if(_activeTab == -1)
        _activeTab = index;

    if(_activeTab != index){
        removeIconAt(_activeTab);
        _activeTab = index;
    }

    QIcon icon;
    icon.addFile(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_174_pause.png"), QSize(), QIcon::Normal, QIcon::Off);
    setTabIcon(index, icon);
}


void PlaylistTabWidget::iconStopAt(int index)
{
    if(_activeTab == -1)
        _activeTab = index;

    if(_activeTab != index){
        removeIconAt(_activeTab);
        _activeTab = index;
    }

    QIcon icon;
    icon.addFile(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_175_stop.png"), QSize(), QIcon::Normal, QIcon::Off);
    setTabIcon(index, icon);
}

void PlaylistTabWidget::removeIconAt(int index)
{
   QIcon icon;
   icon.addFile(QString::fromUtf8(":/icons/resources/glyphicons/empty.png"), QSize(), QIcon::Normal, QIcon::Off);
   setTabIcon(index, icon);
}
