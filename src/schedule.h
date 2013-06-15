#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QObject>
#include <QDateTime>
#include <QTimer>

#include "playlist.h"

class Schedule : public QObject
{
    Q_OBJECT
public:
    explicit Schedule(Playlist *playlist, const QDateTime &launchAt, QObject *parent = 0);
    virtual ~Schedule();

    inline Playlist* playlist() const { return _playlist; }


    /**
     * @brief start
     */
    void start();

    /**
     * @brief stop
     */
    void stop();

    /**
     * @brief cancel
     */
    void cancel();

    /**
     * @brief isExpired
     * @return
     */
    bool isExpired() const;

    /**
     * @brief isActive
     * @return
     */
    bool isActive() const;

    /**
     * @brief wasTriggered
     * @return
     */
    bool wasTriggered() const;

    /**
     * @brief launchAt
     * @return
     */
    inline const QDateTime & launchAt() const { return _launchAt; }

    /**
     * @brief finishAt
     * @return
     */
    QDateTime finishAt() const;

    /**
     * @brief delay
     * @param ms
     */
    void delay(int ms);

signals:

    /**
     * @brief triggered
     * @param playlist
     */
    void triggered(Playlist *playlist);

private slots:

    /**
     * @brief timeout
     */
    void timeout();

private:

    /**
     * @brief _timer
     */
    QTimer _timer;

    /**
     * @brief _launchAt
     */
    QDateTime _launchAt;

    /**
     * @brief _playlist
     */
    Playlist *_playlist;

    /**
     * @brief _wasTriggered
     */
    bool _wasTriggered;
};

#endif // SCHEDULE_H
