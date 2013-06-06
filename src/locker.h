#ifndef LOCKER_H
#define LOCKER_H

#include <QObject>
#include <QTimer>

class Locker : public QObject
{
    Q_OBJECT
public:
    explicit Locker(QObject *parent = 0);

    bool getAutoLock();
    void setAutoLock(bool);
    bool passwordEnable();
    void setPasswordEnable(bool);
    QString getPassword();
    void setPassword(QString);
    void setTimer(int);

    /*
     * auto_lock enable,
     * delay,
     * Qtimer,
     * password,
     * pass enable
     * set, get toussa
     * emit lock() quand timer écoulé
     */

private:
    bool _autoLock;
    bool _password;
    QString _thePass;
    QTimer _timer;
/*signals:
    
public slots:*/
    
};

#endif // LOCKER_H
