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

    /**
     * @brief projectTitle
     * @return
     */
    inline const QString& projectTitle() const { return _projectTitle; }

    /**
     * @brief projectNotes
     * @return
     */
    inline const QString& projectNotes() const { return _projectNotes; }

    /**
     * @brief setMediaListModel
     * @param model
     */
    void setMediaListModel(MediaListModel* model);

    /**
     * @brief addPlaylistModel
     * @param model
     */
    void addPlaylistModel(PlaylistModel* model);

    /**
     * @brief setScheduleListModel
     * @param model
     */
    void setScheduleListModel(ScheduleListModel *model);

    /**
     * @brief save
     * @param file
     */
    void save(QFile &file);

    /**
     * @brief load
     * @param file
     */
    void load(QFile &file);

    /**
     * @brief clear
     */
    void clear();
    
signals:

    /**
     * @brief saved
     */
    void saved();

    /**
     * @brief loaded
     */
    void loaded();

public slots:

    /**
     * @brief setProjectTitle
     * @param title
     */
    void setProjectTitle(const QString &title);

    /**
     * @brief setProjectNotes
     * @param notes
     */
    void setProjectNotes(const QString &notes);

protected:

    /**
     * @brief findPlaylistById
     * @param id
     * @return
     */
    Playlist* findPlaylistById(int id) const;

    /**
     * @brief findMediaById
     * @param id
     * @return
     */
    Media* findMediaById(int id) const;

private:

    /**
     * @brief _mediaListModel
     */
    MediaListModel* _mediaListModel;

    /**
     * @brief _playlistModelList
     */
    QList<PlaylistModel*> _playlistModelList;

    /**
     * @brief _scheduleListModel
     */
    ScheduleListModel* _scheduleListModel;

    /**
     * @brief _projectTitle
     */
    QString _projectTitle;

    /**
     * @brief _projectNotes
     */
    QString _projectNotes;

    /**
     * @brief _app
     */
    Application *_app;

    // FIX : ref 0000001
    MainWindow *_win;
};

#endif // DATASTORAGE_H
