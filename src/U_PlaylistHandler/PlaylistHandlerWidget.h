#ifndef PLAYLISTHANDLERWIDGET_H
#define PLAYLISTHANDLERWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include "mainwindow.h"
#include "PlaylistTabWidget.h"
#include "playlisttableview.h"
#include "PlaylistControlWidget.h"

class PlaylistTableView;
class PlaylistTabWidget;
class PlaylistControlWidget;

class PlaylistHandlerWidget: public QWidget
{
    Q_OBJECT
public:
    PlaylistHandlerWidget(MainWindow* mw, QWidget* parent = 0);

    PlaylistControlWidget* playlistControlWidget() const { return _playlistControlWidget; }

    PlaylistTabWidget* playlistTabWidget() const { return _playlistTabWidget; }

    PlaylistTableView* currentPlaylistTableView() const;

    PlaylistModel* currentPlaylistModel() const;

    /**
     * @brief Returns the playlist at the position index
     * @return The playlist at the position index
     */
    Playlist* playlistAt(int index) const;

private:
    PlaylistTabWidget* _playlistTabWidget;

    PlaylistControlWidget* _playlistControlWidget;
};

#endif // PLAYLISTHANDLERWIDGET_H
