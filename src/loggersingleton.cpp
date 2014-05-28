#include "loggersingleton.h"
#include "iostream"

LoggerSingleton* LoggerSingleton::_single = NULL;
QPointer<QLabel> LoggerSingleton::_textEdit = NULL;
QMutex LoggerSingleton::_mutex;

LoggerSingleton *LoggerSingleton::getInstance()
{
    if(!_single)
    {
        _mutex.lock();
        if (!_single)
            _single = new LoggerSingleton();
        _mutex.unlock();
    }
    return _single;
}

void LoggerSingleton::destroyInstance()
{
    if(_single)
    {
        _mutex.lock();
        delete _single;
        _single = NULL;
       _mutex.unlock();
    }
}

void LoggerSingleton::setTextEdit(QLabel *textEdit)
{
    _textEdit = textEdit;
}

void LoggerSingleton::writeMessage(const QString &message)
{
    if(_textEdit)
    {
        _mutex.lock();
        _textEdit->setText(_textEdit->text()+"\n"+message);
        _mutex.unlock();
    }
}
