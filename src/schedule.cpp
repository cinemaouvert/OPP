#include "schedule.h"

#include "playlist.h"

Schedule::Schedule(Playlist *playlist, const QDateTime &launchAt, QObject *parent) :
    QObject(parent),
    _playlist(playlist),
    _launchAt(launchAt)
{
}

Schedule::~Schedule()
{
}

Schedule::Schedule(const Schedule &other)
{
    _playlist = other._playlist;
    _launchAt = other._launchAt;
}

Schedule & Schedule::operator=(const Schedule &other)
{
    if (this != &other) {
        _playlist = other._playlist;
        _launchAt = other._launchAt;
    }
    return *this;
}


QDateTime Schedule::finishAt() const
{
    return _launchAt.addMSecs(_playlist->totalDuration());
}

void Schedule::start()
{
    if (isExpired() || isActive())
        return;

    _timer.singleShot(QDateTime::currentDateTime().msecsTo(_launchAt), this, SLOT(timeout()));
}

void Schedule::stop()
{
    _timer.stop();
}

bool Schedule::isExpired() const
{
    return _launchAt < QDateTime::currentDateTime();
}

bool Schedule::isActive() const
{
    return _timer.isActive();
}

void Schedule::delay(int ms)
{
    bool wasActived = isActive();
    stop();

    _launchAt = _launchAt.addMSecs(ms);

    if (wasActived)
        start();
}

void Schedule::timeout()
{
    emit triggered(_playlist);
}
