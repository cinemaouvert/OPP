#include "locker.h"

#include <QWidget>
#include <QDebug>

Locker::Locker(QList<QWidget*> widgets, QObject *parent) :
    QObject(parent),
    _widgets(widgets)
{
}

bool Locker::getAutoLock(){
    return _autoLock;
}

void Locker::setAutoLock(bool lock){
    _autoLock = lock;
    if(lock == false){
        _timer.stop();
    }
}

bool Locker::passwordEnable(){
    return _password;
}

void Locker::setPasswordEnable(bool pass){
    _password = pass;
}

QString Locker::getPassword(){
    return _thePass;
}

void Locker::setPassword(QString newPass){
    if(passwordEnable()==true){
        _thePass = newPass;
    }
}

void Locker::setAutoLockDelay(int time){
    if(getAutoLock()==true){
        // attention, ça va lancer le timer indéfiniment.
        // Tout les "time" millisecondes, le timer emet un signal timeout
        // pour emettre un seul timeout, il faut utiliser singleShot()
        _timer.start(time);
    }
}

void Locker::lock()
{
    qDebug() << "locked";
    // pour chaque widgets enregistré dans _widgets
    //      faire un setEnabled(false)
}

void Locker::unlock()
{
    qDebug() << "unlocked";
    // pour chaque widgets enregistré dans _widgets
    //      faire un setEnabled(true)
}

void Locker::toggle(bool checked)
{
    checked ? lock() : unlock();

    // ca permet de connecté la methode toggle à un signal toggled emis par un clique bouton
}
