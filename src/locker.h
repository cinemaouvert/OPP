#ifndef LOCKER_H
#define LOCKER_H

#include <QObject>
#include <QTimer>

class Locker : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Locker
     * @param widgets The widgets to lock and unlock
     * @param parent
     */
    explicit Locker(QList<QWidget*> widgets, QObject *parent = 0);

    bool getAutoLock();
    void setAutoLock(bool);
    bool passwordEnable();
    void setPasswordEnable(bool);
    QString getPassword();
    void setPassword(QString);
    void setAutoLockDelay(int);

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
    QList<QWidget*> _widgets;

    /**
     * @brief delay in ms
     */
    int delay;

public slots:
    void lock();
    void unlock();

    /**
     * Toggle lock state.
     * If the locker is locked, then it is unlocked and if it is unlocked, it is locked.
     * @brief toggle
     */
    void toggle(bool checked = false);

signals:
    void autoLocked();
    void locked();
    void unlocked();
    void toggled();
    
};

#endif // LOCKER_H
