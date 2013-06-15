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

    Schedule(const Schedule &other);
    Schedule & operator=(const Schedule &other);

    inline Playlist* playlist() const { return _playlist; }

    void start();
    void stop();
    void cancel();

    bool isExpired() const;
    bool isActive() const;
    bool wasTriggered() const;

    inline const QDateTime & launchAt() const { return _launchAt; }
    QDateTime finishAt() const;

    void delay(int ms);

signals:
    void triggered(Playlist *playlist);

private slots:
    void timeout();

private:
    QTimer _timer;
    QDateTime _launchAt;

    Playlist *_playlist;
    bool _wasTriggered;
};

#endif // SCHEDULE_H
