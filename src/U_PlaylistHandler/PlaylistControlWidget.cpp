#include "PlaylistControlWidget.h"

PlaylistControlWidget::PlaylistControlWidget(MainWindow* mw, QWidget* parent):
    QVBoxLayout()
{
    setSpacing(0);
    setContentsMargins(0, 0, 4, 0);

    _mw = mw;
    _parent = (PlaylistHandlerWidget*) parent;

    // Size policy
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);

    // Add playlist button
    _addPlaylistButton = new QPushButton();
    sizePolicy.setHeightForWidth(_addPlaylistButton->sizePolicy().hasHeightForWidth());
    _addPlaylistButton->setSizePolicy(sizePolicy);
    _addPlaylistButton->setMaximumSize(QSize(30, 16777215));
    _addPlaylistButton->setStyleSheet(QString(
        "border-top: 1px solid rgb(180, 180, 180);"
        "border-left: 1px solid rgb(180, 180, 180);"
        "border-right: 1px solid rgb(180, 180, 180);"
        "border-bottom: 0px;"
        "background: rgba(0,0,0,0);"
    #if defined(Q_OS_MAC)
        "margin-right: 5px;"
    #endif
    ));

    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_190_circle_plus.png"), QSize(), QIcon::Normal, QIcon::Off);
    _addPlaylistButton->setIcon(icon1);

    addWidget(_addPlaylistButton);

    // Edit name playlist button
    _editNamePlaylistButton = new QPushButton();
    sizePolicy.setHeightForWidth(_editNamePlaylistButton->sizePolicy().hasHeightForWidth());
    _editNamePlaylistButton->setSizePolicy(sizePolicy);
    QIcon icon2;
    icon2.addFile(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_030_pencil.png"), QSize(), QIcon::Normal, QIcon::Off);
    _editNamePlaylistButton->setIcon(icon2);
    _editNamePlaylistButton->setMaximumSize(QSize(30, 16777215));
    _editNamePlaylistButton->setStyleSheet(QString(
        "border-left: 1px solid rgb(180, 180, 180);"
        "border-right: 1px solid rgb(180, 180, 180);"
        "border-top: 0px;"
        "border-bottom: 1px solid rgb(180, 180, 180);"
        "background: rgba(0,0,0,0);"
    #if defined(Q_OS_MAC)
        "margin: 0px 5px 0px 0px;"
    #else
        "margin: 0px 0px 4px 0px;"
    #endif
    ));

    addWidget(_editNamePlaylistButton);

    // Playlist up button
    _playlistUpButton = new QPushButton();
    sizePolicy.setHeightForWidth(_playlistUpButton->sizePolicy().hasHeightForWidth());
    _playlistUpButton->setSizePolicy(sizePolicy);
    QIcon icon3;
    icon3.addFile(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_213_up_arrow.png"), QSize(), QIcon::Normal, QIcon::Off);
    _playlistUpButton->setIcon(icon3);
    _playlistUpButton->setMaximumSize(QSize(30, 16777215));
    _playlistUpButton->setStyleSheet(QString(
        "border-top: 1px solid rgb(180, 180, 180);"
        "border-left: 1px solid rgb(180, 180, 180);"
        "border-right: 1px solid rgb(180, 180, 180);"
        "border-bottom: 0px;"
        "background: rgba(0,0,0,0);"
    #if defined(Q_OS_MAC)
        "margin-right: 5px;"
        "margin-top: 10px;"
    #endif
    ));

    addWidget(_playlistUpButton);

    // Delete playlist button
    _deletePlaylistItemButton = new QPushButton();
    sizePolicy.setHeightForWidth(_deletePlaylistItemButton->sizePolicy().hasHeightForWidth());
    _deletePlaylistItemButton->setSizePolicy(sizePolicy);
    QIcon icon4;
    icon4.addFile(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_016_bin.png"), QSize(), QIcon::Normal, QIcon::Off);
    _deletePlaylistItemButton->setIcon(icon4);
    _deletePlaylistItemButton->setMaximumSize(QSize(30, 16777215));
    _deletePlaylistItemButton->setStyleSheet(QString(
        "border-left: 1px solid rgb(180, 180, 180);"
        "border-right: 1px solid rgb(180, 180, 180);"
        "border-top: 0px;"
        "border-bottom: 0px;"
        "background: rgba(0,0,0,0);"
    #if defined(Q_OS_MAC)
        "margin-right: 5px;"
    #endif
    ));

    addWidget(_deletePlaylistItemButton);

    // Playlist Down Button
    _playlistDownButton = new QPushButton();
    sizePolicy.setHeightForWidth(_playlistDownButton->sizePolicy().hasHeightForWidth());
    _playlistDownButton->setSizePolicy(sizePolicy);
    QIcon icon5;
    icon5.addFile(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_212_down_arrow.png"), QSize(), QIcon::Normal, QIcon::Off);
    _playlistDownButton->setIcon(icon5);
    _playlistDownButton->setMaximumSize(QSize(30, 16777215));
    _playlistDownButton->setStyleSheet(QString(
        "border-left: 1px solid rgb(180, 180, 180);"
        "border-right: 1px solid rgb(180, 180, 180);"
        "border-top: 0px;"
        "border-bottom: 1px solid rgb(180, 180, 180);"
        "background: rgba(0,0,0,0);"
    #if defined(Q_OS_MAC)
        "margin-right: 5px;"
    #endif
    ));

    addWidget(_playlistDownButton);

    /********* Connections **************/
    connect(_addPlaylistButton, SIGNAL(clicked()), this, SLOT(addPlaylist()));
    connect(_playlistUpButton, SIGNAL(clicked()), this, SLOT(playlistUp()));
    connect(_playlistDownButton, SIGNAL(clicked()), this, SLOT(playlistDown()));
    connect(_editNamePlaylistButton, SIGNAL(clicked()), this, SLOT(renamePlaylist()));
    connect(_deletePlaylistItemButton, SIGNAL(clicked()), this, SLOT(deletePlaylistItem()));
}

PlaylistControlWidget::~PlaylistControlWidget()
{
}

void PlaylistControlWidget::addPlaylist()
{
    if(!_mw->locker()->isLock()) {
        bool ok;

        QString text = QInputDialog::getText(_mw,
                                             tr("New playlist"),
                                             tr("Playlist title : "),
                                             QLineEdit::Normal,
                                             tr("New playlist"),
                                             &ok
                                             );
        if(ok) {
            _mw->playlistTabWidget()->createTab(text);
        }
    }
    else
        QMessageBox::critical(_mw, tr("Add new playlist"), tr("The playlist is currently locked, you can not add a new playlist.") , tr("OK"));
}

void PlaylistControlWidget::renamePlaylist()
{
    _parent->playlistTabWidget()->editPlaylistName();
}

void PlaylistControlWidget::removePlaylistItem()
{
    deletePlaylistItem();
}

void PlaylistControlWidget::playlistUp()
{
    QModelIndexList indexes = _parent->currentPlaylistTableView()->selectionModel()->selectedRows();
    PlaylistPlayer* playlistPlayer = _mw->playlistPlayer();

    if(indexes.count()==0)
        return;

    if(_parent->currentPlaylistModel()->moveUp(indexes.first())){
        _parent->currentPlaylistTableView()->setCurrentIndex(_parent->currentPlaylistModel()->index(indexes.first().row() - 1, indexes.first().column()));
        if(playlistPlayer->getCurrentIndex() == ((QModelIndex)indexes.first()).row())
            playlistPlayer->currentIndexDown();
        else if (playlistPlayer->getCurrentIndex() == ((QModelIndex)indexes.first()).row()-1)
            playlistPlayer->currentIndexUp();
    }
}

void PlaylistControlWidget::playlistDown()
{
    QModelIndexList indexes = _parent->currentPlaylistTableView()->selectionModel()->selectedRows();
    PlaylistPlayer* playlistPlayer = _mw->playlistPlayer();

    if(indexes.count() == 0)
        return;

    if(_parent->currentPlaylistModel()->moveDown(indexes.first())){
        _parent->currentPlaylistTableView()->setCurrentIndex(_parent->currentPlaylistModel()->index(indexes.first().row() + 1, indexes.first().column()));
        if(playlistPlayer->getCurrentIndex() == ((QModelIndex)indexes.first()).row())
           playlistPlayer->currentIndexUp();
        else if (playlistPlayer->getCurrentIndex() == ((QModelIndex)indexes.first()).row()+1)
           playlistPlayer->currentIndexDown();
    }
}

void PlaylistControlWidget::deletePlaylistItem()
{
    if(_mw->locker()->isLock())
        QMessageBox::critical(_mw, tr("Delete playlist item"), tr("This playlist is currently locked, you can not delete an item.") , tr("OK"));
    else {
        QModelIndexList indexes = _parent->currentPlaylistTableView()->selectionModel()->selectedRows();

        if (indexes.count() == 0)
            return;
        if(!_parent->currentPlaylistModel()->isRunningMedia(indexes.first().row())){
            _parent->currentPlaylistModel()->removePlayback(indexes.first().row());
            _mw->updateSettings();
            _mw->scheduleListModel()->updateLayout();
        }else{
            QMessageBox::critical(_mw, tr("Remove item"), tr("This playlist is currently running, you can not delete media that have been or are being displayed.") , tr("OK"));
        }
    }

    if(_parent->currentPlaylistTableView() != NULL && _parent->currentPlaylistTableView()->selectionModel() != NULL){
        QModelIndexList indexes = _parent->currentPlaylistTableView()->selectionModel()->selectedRows();
        if(indexes.count()>0)
            _mw->setSelectedMediaTimeByIndex(indexes.first().row());
    }
}
