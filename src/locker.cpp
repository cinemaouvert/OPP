#include "locker.h"

Locker::Locker(QObject *parent) :
    QObject(parent)
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

void Locker::setTimer(int time){
    if(getAutoLock()==true){
        _timer.start(1000*time);
    }
}
