#ifndef CUSTOMEVENTFILTER_H
#define CUSTOMEVENTFILTER_H

#include "locker.h"

#include <QObject>
#include <QEvent>
#include <QTimer>

class CustomEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit CustomEventFilter(Locker* lock, QObject *parent = 0);
    virtual ~CustomEventFilter();
    bool eventFilter(QObject * o, QEvent * e);

private:

    QTimer* _timer;
    Locker* _lock;
    
};

#endif // CUSTOMEVENTFILTER_H
