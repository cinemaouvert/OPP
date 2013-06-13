#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractTableModel>
#include <QList>

#include "playlist.h"

class PlaylistPlayer;
class Playback;
class MediaListModel;

class PlaylistModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Columns { Title = 0, Duration = 1, Video = 2, Audio = 3, Subtitles = 4, TestPattern = 5, Status = 6 };
    enum PlaybackState { Playing = 0, Paused = 1, Idle = 2 };

    PlaylistModel(Playlist *playlist, MediaListModel *mediaListModel, QObject *parent = 0);

    virtual ~PlaylistModel();

    inline Playlist* playlist() const { return _playlist; }

    int activeItemIndex() const { return _activeItem.first; }

    int columnCount(const QModelIndex &parent) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QVariant data(const QModelIndex &index, int role) const;

    bool addPlayback(Playback *playback);

    void removePlaybackWithDeps(Media *media);

    void removePlayback(int index);

    bool dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent );

    void updateLayout();

public slots:

    void playItem();

    void pauseItem();

    void stopItem();

    void setPlayingItem(int index);

private:
    Playlist *_playlist;
    MediaListModel *_mediaListModel;
    QPair<int, PlaybackState> _activeItem;
};


#endif // PLAYLISTMODEL_H
