#ifndef LOCKER_H
#define LOCKER_H

#include <QObject>
#include <QTimer>
#include <QDialog>

class passformdialog;

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

    /**
     * @brief getAutoLock
     * @return
     */
    bool getAutoLock();

    /**
     * @brief setAutoLock
     */
    void setAutoLock(bool);

    /**
     * @brief passwordEnable
     * @return
     */
    bool passwordEnable();

    /**
     * @brief setPasswordEnable
     */
    void setPasswordEnable(bool);

    /**
     * @brief getPassword
     * @return
     */
    QString getPassword();

    /**
     * @brief setPassword
     */
    void setPassword(QString);

    /**
     * @brief setAutoLockDelay
     */
    void setAutoLockDelay(int);

    /**
     * @brief getWidgets
     * @return
     */
    QList<QWidget*> getWidgets();

private:

    /**
     * @brief _autoLock
     */
    bool _autoLock;

    /**
     * @brief _password
     */
    bool _password;

    /**
     * @brief _thePass
     */
    QString _thePass;

    /**
     * @brief _timer
     */
    QTimer _timer;

    /**
     * @brief _widgets
     */
    QList<QWidget*> _widgets;

    /**
     * @brief _passDialog
     */
    passformdialog *_passDialog;

    /**
     * @brief delay in ms
     */
    int delay;

public slots:

    /**
     * @brief lock
     */
    void lock();

    /**
     * @brief unlock
     */
    void unlock();

    /**
     * Toggle lock state.
     * If the locker is locked, then it is unlocked and if it is unlocked, it is locked.
     * @brief toggle
     */
    void toggle(bool checked = false);

signals:
    /**
     * @brief autoLocked
     */
    void autoLocked();

    /**
     * @brief locked
     */
    void locked();

    /**
     * @brief unlocked
     */
    void unlocked();

    /**
     * @brief toggled
     * @param checked
     */
    void toggled(bool checked);
    
};

#endif // LOCKER_H
