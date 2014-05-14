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

#include "statuswidget.h"
#include "ui_statuswidget.h"

#include <QDateTime>

StatusWidget::StatusWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusWidget)
{
    ui->setupUi(this);

    _timerId = startTimer(900);
    setMediaCount(0);

    QIcon icon;

    icon = QIcon(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_054_clock.png"));
    ui->timeIconLabel->setPixmap(icon.pixmap(16));

    icon = QIcon(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_045_calendar.png"));
    ui->dateIconLabel->setPixmap(icon.pixmap(16));
}

StatusWidget::~StatusWidget()
{
    killTimer(_timerId);
    delete ui;
}

void StatusWidget::setMediaCount(int count)
{
    ui->countLoadedFilesLabel->setText(QString("%1 %2")
        .arg( QString::number(count) )
        .arg( tr("files loaded") )
    );
}

void StatusWidget::timerEvent(QTimerEvent *event)
{
    QDateTime now = QDateTime::currentDateTime();

    ui->timeLabel->setText(now.time().toString("hh:mm:ss"));
    ui->dateLabel->setText(now.date().toString("dd/MM/yy"));
}

