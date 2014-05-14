/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
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

#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QWidget>

namespace Ui {
class StatusWidget;
}

class StatusWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit StatusWidget(QWidget *parent = 0);
    ~StatusWidget();
    
public slots:

    /**
     * @brief Set the value of media counter
     * @param count The new value of media counter
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void setMediaCount(int count);

protected:
    /**
     * @brief Update the time
     * @param event
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void timerEvent(QTimerEvent *event);

private slots:

private:
    /**
     * @brief ui The UI
     */
    Ui::StatusWidget *ui;

    /**
     * @brief ui The timer identifiant
     */
    int _timerId;
};

#endif // STATUSWIDGET_H
