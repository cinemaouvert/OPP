/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Thomas Berthomé <thoberthome@laposte.net>
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

#ifndef LOGGERSINGLETON_H
#define LOGGERSINGLETON_H

#include <QLabel>
#include <QString>
#include <QPointer>
#include <QMutex>

class LoggerSingleton
{
public:
    /**
     * @brief Get the instance
     *
     * @author Thomas Berthome <thoberthome@laposte.net>
     */
    static LoggerSingleton *getInstance();

    /**
     * @brief Delete the instance
     *
     * @author Thomas Berthome <thoberthome@laposte.net>
     */
    static void destroyInstance();

    /**
     * @brief Set text edit
     * @param textEdit The text edit
     *
     * @author Thomas Berthome <thoberthome@laposte.net>
     */
    static void setTextEdit(QLabel *textEdit);

    /**
     * @brief Set the message in the text edit
     * @param message The message
     *
     * @author Thomas Berthome <thoberthome@laposte.net>
     */
    static void writeMessage(const QString &message);
private:
    LoggerSingleton(){}
    ~LoggerSingleton(){}

    /**
     * @brief _single The instance
     *
     */
    static LoggerSingleton* _single;

    /**
     * @brief _textEdit Pointer on the textEdit
     *
     */
    static QPointer<QLabel> _textEdit;

    /**
     * @brief _mutex The mutex
     *
     */
    static QMutex _mutex;
};

#endif // LOGGERSINGLETON_H
