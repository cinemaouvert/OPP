#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>

#include "playback.h"

struct libvlc_media_list_t;
struct libvlc_instance_t;

class Playlist : public QObject
{
    Q_OBJECT
public:
    Playlist(libvlc_instance_t *vlcInstance, const QString &title, QObject *parent = 0);
    virtual ~Playlist();

    /**
     * @brief playbackList
     * @return
     */
    inline const QList<Playback*>& playbackList() { return _playbackList; }

    /**
     * @brief id
     * @return
     */
    inline int id() const { return _id; }

    /**
     * @brief core
     * @return
     */
    inline libvlc_media_list_t* core() const { return _vlcMediaList; }

    /**
     * @brief title
     * @return
     */
    inline QString title() const { return _title; }

    /**
     * @brief setId
     * @param id
     */
    void setId(int id);

    /**
     * @brief setTitle
     * @param title
     */
    void setTitle(const QString &title);

    /**
     * @brief at
     * @param index
     * @return
     */
    Playback* at(const int &index) const;

    /**
     * @brief append
     * @param playback
     */
    void append(Playback *playback);

    /**
     * @brief removeAt
     * @param index
     */
    void removeAt(int index);

    /**
     * @brief indexOf
     * @param playback
     * @return
     */
    int indexOf(Playback *playback) const;

    /**
     * @brief move
     * @param from
     * @param to
     */
    void move(int from, int to);

    /**
     * @brief count
     * @return
     */
    int count() const;

    /**
     * @brief totalDuration
     * @return
     */
    uint totalDuration() const;

    /**
     * @brief lock
     */
    void lock();

    /**
     * @brief unlock
     */
    void unlock();

signals:

    /**
     * @brief titleChanged
     */
    void titleChanged();

private:

    /**
     * @brief s_instanceCount
     */
    static int s_instanceCount;

    /**
     * @brief _id
     */
    int _id;

    /**
     * @brief _playbackList
     */
    QList<Playback*> _playbackList;

    /**
     * @brief _vlcMediaList
     */
    libvlc_media_list_t *_vlcMediaList;

    /**
     * @brief _title
     */
    QString _title;
};

#endif // PLAYLIST_H
