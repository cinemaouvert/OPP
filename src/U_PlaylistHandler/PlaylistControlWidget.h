#ifndef PLAYLISTCONTROLWIDGET_H
#define PLAYLISTCONTROLWIDGET_H

#include <QInputDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFrame>

#include "mainwindow.h"
#include "PlaylistHandlerWidget.h"
#include "playlisttableview.h"
#include "locker.h"

class PlaylistHandlerWidget;

class PlaylistControlWidget: public QVBoxLayout
{
    Q_OBJECT
public:
    PlaylistControlWidget(MainWindow* mw, QWidget* parent = 0);
    virtual ~PlaylistControlWidget();

public slots:
    void addPlaylist();

    void renamePlaylist();

    void removePlaylistItem();

    void playlistUp();

    void playlistDown();

    void deletePlaylistItem();

private:
    MainWindow* _mw;

    PlaylistHandlerWidget* _parent;

    QPushButton* _addPlaylistButton;

    QPushButton* _editNamePlaylistButton;

    QPushButton* _playlistUpButton;

    QPushButton* _playlistDownButton;

    QPushButton* _deletePlaylistItemButton;
};

#endif // PLAYLISTCONTROLWIDGET_H
