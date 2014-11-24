#ifndef PLAYLISTTABWIDGET_H
#define PLAYLISTTABWIDGET_H

#include <QTabWidget>
#include <QMessageBox>

#include "mainwindow.h"
#include "PlaylistPlayer.h"
#include "playlistmodel.h"

class PlaylistModel;

class PlaylistTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    PlaylistTabWidget(MainWindow *mw, QWidget* parent = 0);

    void createTab(QString name = NULL);

    void restoreTab(PlaylistModel *model);

    void editPlaylistName();

    void iconPlayAt(int index);

    void iconPauseAt(int index);

    void iconStopAt(int index);

    void removeIconAt(int index);

private:
    void deconnection();

    void connection();

private slots:
    void tabCloseRequested(int index);

    void currentChanged(int index);

    void connectionCurrentTab();

private:
    MainWindow* _mw;

    PlaylistPlayer* _playlistPlayer;

    int _lastSelectedTab;

    int _activeTab;
};

#endif // PLAYLISTTABWIDGET_H
