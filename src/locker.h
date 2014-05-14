/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Baptiste Rozière <bapt.roziere@gmail.com>
 *          Florian Mhun <florian.mhun@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
 *
 * Open Projection Program is an initiative of Catalogue Ouvert du Cinéma.
 * The software was developed by four students of University of Poitiers
 * as school project.
 *
 * Open Projection Program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Open Projection Program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Open Projection Program. If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************************/

#ifndef LOCKER_H
#define LOCKER_H

#include <QObject>
#include <QTimer>
#include <QDialog>

class Locker : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Locker
     * @param widgets The widgets to lock and unlock
     * @param parent
     *
     * @author Baptiste Rozière <bapt.roziere@gmail.com>
     */
    explicit Locker(QList<QWidget*> widgets, QObject *parent = 0);
    virtual ~Locker();

    /**
     * @brief getAutoLock
     * @return True if auto lock is enabled, false otherwise
     *
     * @author Baptiste Rozière <bapt.roziere@gmail.com>
     */
    bool getAutoLock();

    /**
     * @brief setAutoLock
     *
     * @author Baptiste Rozière <bapt.roziere@gmail.com>
     */
    void setAutoLock(bool);

    /**
     * @brief passwordEnable
     * @return True if password is enabled, false otherwise
     *
     * @author Baptiste Rozière <bapt.roziere@gmail.com>
     */
    bool passwordEnable();

    /**
     * @brief setPasswordEnable
     *
     * @author Baptiste Rozière <bapt.roziere@gmail.com>
     */
    void setPasswordEnable(bool);

    /**
     * @brief getPassword
     * @return The password
     *
     * @author Baptiste Rozière <bapt.roziere@gmail.com>
     */
    QString getPassword();

    /**
     * @brief setPassword
     *
     * @author Baptiste Rozière <bapt.roziere@gmail.com>
     */
    void setPassword(QString);

    /**
     * @brief setAutoLockDelay
     *
     * @author Baptiste Rozière <bapt.roziere@gmail.com>
     */
    void setAutoLockDelay(int);

    /**
     * @brief getWidgets
     * @return The list of lockable widgets
     *
     * @author Baptiste Rozière <bapt.roziere@gmail.com>
     */
    QList<QWidget*> getWidgets();

    /**
     * @brief getLock
     * @return true if the playlist is lock, false otherwise
     *
     * @author Geoffrey Bergé <geoffrey.berge@live.fr>
     */
    bool isLock();

    /**
     * @brief getTime
     * @return time time for autolock
     *
     * @author Geoffrey Bergé <geoffrey.berge@live.fr>
     */
    int getTime();



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
     * @brief _time
     */
    int _time;

    /**
     * @brief _widgets
     */
    QList<QWidget*> _widgets;

    /**
     * @brief _lock
     */
    int _lock;

public slots:

    /**
     * @brief autoLock
     *
     * @author Geoffrey Bergé <geoffrey.berge@live.fr>
     */
    void autoLock();

    /**
     * @brief Lock
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void lock();

    /**
     * @brief Unlock
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void unlock();

    /**
     * @brief Toggle lock state. If the locker is locked, then it is unlocked and if it is unlocked, it is locked.
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
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
