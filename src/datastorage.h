#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <QObject>
#include <QString>
#include <QFile>

class MediaListModel;
class PlaylistModel;
class ScheduleListModel;
class Application;

// FIX : ref 0000001
class MainWindow;

class DataStorage : public QObject
{
    Q_OBJECT
public:
    explicit DataStorage(Application *app, MainWindow *win/*FIX : ref 0000001*/, QObject *parent = 0);

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
    Application *_app;

    // FIX : ref 0000001
    MainWindow *_win;
};

#endif // DATASTORAGE_H
