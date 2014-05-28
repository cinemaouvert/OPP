#ifndef LOGGERSINGLETON_H
#define LOGGERSINGLETON_H

#include <QLabel>
#include <QString>
#include <QPointer>
#include <QMutex>

class LoggerSingleton
{
    public:
        static LoggerSingleton *getInstance();
        static void destroyInstance();
        static void setTextEdit(QLabel *textEdit);
        static void writeMessage(const QString &message);
    private:
        LoggerSingleton(){}
        ~LoggerSingleton(){}
        static LoggerSingleton* _single;
        static QPointer<QLabel> _textEdit;
        static QMutex _mutex;
};

#endif // LOGGERSINGLETON_H
