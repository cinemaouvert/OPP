#include "customeventfilter.h"
#include <QDebug>

CustomEventFilter::CustomEventFilter(Locker* lock, QObject *parent) :
    QObject(parent)
{
    _lock = lock;
    _timer = new QTimer();
}

CustomEventFilter::~CustomEventFilter()
{
    delete _timer;
}

bool CustomEventFilter::eventFilter(QObject * o, QEvent * e)
{
    if(e->type() == QEvent::MouseMove && _lock->getAutoLock() && _lock->getTime()!=-1){
        _timer->connect(_timer, SIGNAL(timeout()), _lock, SLOT(autoLock()));
        _timer->start(_lock->getTime()*60);
    }
    return false;
}

