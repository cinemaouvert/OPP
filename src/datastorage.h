#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <QObject>
#include <QString>
#include <QFile>

class MediaListModel;
class PlaylistModel;
class ScheduleListModel;

class DataStorage : public QObject
{
    Q_OBJECT
public:
    explicit DataStorage(QObject *parent = 0);

    void setMediaListModel(MediaListModel* model);
    void addPlaylistModel(PlaylistModel* model);
    void setScheduleListModel(ScheduleListModel *model);

    void save(QFile &file);
    void load(const QFile &file);
    
signals:
    
public slots:
    void setProjectTitle(const QString &title);
    void setProjectNotes(const QString &notes);

private :
    MediaListModel* _mediaListModel;
    QList<PlaylistModel*> _playlistModelList;
    ScheduleListModel* _scheduleListModel;
    QString _projectTitle;
    QString _projectNotes;
    
};

#endif // DATASTORAGE_H
