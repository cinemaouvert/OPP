#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <QObject>
#include <QString>
#include <QFile>

class MediaListModel;
class PlaylistModel;
class ScheduleListModel;
class Application;
class Media;
class Playlist;

// FIX : ref 0000001
class MainWindow;

class DataStorage : public QObject
{
    Q_OBJECT
public:
    explicit DataStorage(Application *app, MainWindow *win/*FIX : ref 0000001*/, QObject *parent = 0);

    inline const QString& projectTitle() const { return _projectTitle; }
    inline const QString& projectNotes() const { return _projectNotes; }

    void setMediaListModel(MediaListModel* model);
    void addPlaylistModel(PlaylistModel* model);
    void setScheduleListModel(ScheduleListModel *model);

    void save(QFile &file);
    void load(QFile &file);
    void clear();
    
signals:
    void saved();
    void loaded();

public slots:
    void setProjectTitle(const QString &title);
    void setProjectNotes(const QString &notes);

protected:
    Playlist* findPlaylistById(int id) const;
    Media* findMediaById(int id) const;

private:
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
