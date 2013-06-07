#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractTableModel>
#include <QList>

#include "playlist.h"

class Playback;
class MediaListModel;

class PlaylistModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Columns { Title = 0, Duration = 1, Video = 2, Audio = 3, Subtitles = 4, TestPattern = 5, Gain = 6, Status = 7 };

    PlaylistModel(MediaListModel *mediaListModel, QObject *parent = 0);

    inline const Playlist & playlist() const { return _playlist; }

    int columnCount(const QModelIndex &parent) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QVariant data(const QModelIndex &index, int role) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole);

    bool addPlayback(Playback *playback);

    void removePlaybackWithDeps(Media *media);

    void removePlayback(int index);

    bool dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent );
private:
    Playlist _playlist;
    MediaListModel *_mediaListModel;
};


#endif // PLAYLISTMODEL_H
