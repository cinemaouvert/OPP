/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Denis Saunier <saunier.denis.86@gmail.com>
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
#include "updater.h"

Updater::Updater(QObject *parent) :
    QObject(parent)
{

}

Updater::~Updater(){

}

void Updater::checkVersion(){
    QNetworkAccessManager manager(this);

    QUrl url_version (config_opp::URL + config_opp::LINK_UPDATE + "version.txt");
    QNetworkRequest request(url_version);
    QNetworkReply *reply = manager.get(request);

    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

    loop.exec();
    if (reply->error() == QNetworkReply::NoError){
       QString versionNew = reply->readAll();
       if (versionNew != config_opp::VERSION){
           if (QMessageBox::Yes == QMessageBox::question(NULL,
                                                         tr("Update OPP"),
                                                         tr("A new version of opp is available, do you want download it ?\nSave your modification before update."
                                                            ),
                                                         QMessageBox::Yes|QMessageBox::No))
               this->getInstaller();
       }
    }else{
        QMessageBox::warning(NULL, tr("Update OPP"), tr("Update unreachable, check your connection"));
    };
    reply->deleteLater();
}

void Updater::getInstaller(){
    QNetworkAccessManager manager(this);

    QFile * file = new QFile ( QString(config_opp::NAME_INSTALLER), this );
    file->open(QIODevice::WriteOnly);

    QUrl url_version (config_opp::URL + config_opp::LINK_UPDATE + config_opp::NAME_INSTALLER);
    QNetworkRequest request(url_version);
    QNetworkReply *reply = manager.get(request);

    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

    loop.exec();
    if (reply->error() == QNetworkReply::NoError){
        file->write(reply->readAll());
    }

    file->close();
    reply->deleteLater();

    //Launch installer
    #if defined(Q_OS_WIN)
        QProcess::startDetached(config_opp::NAME_INSTALLER);
        //exit(0);
    #else
        QDesktopServices::openUrl(QUrl(config_opp::URL + config_opp::LINK_UPDATE + config_opp::NAME_INSTALLER));

    #endif
}


