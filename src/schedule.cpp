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
    if (isExpired() || _timer.isActive())
        return;

    _timer.singleShot(_launchAt.msecsTo(QDateTime::currentDateTime()), this, SLOT(timeout()));
}

void Schedule::stop()
{
    _timer.stop();
}

bool Schedule::isExpired() const
{
    return _launchAt < QDateTime::currentDateTime();
}

void Schedule::timeout()
{
    emit triggered(_playlist);
}
