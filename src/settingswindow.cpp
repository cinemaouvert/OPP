/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Cyril Naud <futuramath@gmail.com>
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

#include "settingswindow.h"
#include "ui_settingswindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QSettings>
#include <QString>
#include <QFileDialog>
#include <QDir>

#include <string.h>
#include <sstream>

SettingsWindow::SettingsWindow(QWidget *parent, MediaPlayer *mp) :
    QDialog(parent),
    _mediaPlayer(mp),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    /*Settings*/
    QSettings settings("opp","opp");
    ui->lineEdit_VLCPath->setText(settings.value("vlcPath").toString());
    ui->lineEdit_testPatternPath->setText(settings.value("testPatternPath").toString());
    ui->lineEdit_intertitlePath->setText(settings.value("intertitlePath").toString());
    ui->lineEdit_moviesPath->setText(settings.value("moviesPath").toString());
    ui->radioButton_locateRight->setChecked(true);
    ui->radioButton_locateLeft->setChecked(false);

    ui->comboBox_language->setCurrentIndex(getIndex(settings.value("lang").toString()));
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

int SettingsWindow::getIndex(QString lang)
{
    if(lang=="en")
        return 0;
    else if(lang=="fr")
        return 1;
}

QString SettingsWindow::getLang(int index)
{
    switch(index)
    {
        case 0:
            return "en";
            break;
        case 1:
            return "fr";
            break;
        default:
            return "en";
    }
}

void SettingsWindow::on_buttonBox_accepted()
{
    QSettings settings("opp","opp");
    settings.setValue("vlcPath", ui->lineEdit_VLCPath->text());
    settings.setValue("testPatternPath", ui->lineEdit_testPatternPath->text());
    settings.setValue("intertitlePath", ui->lineEdit_intertitlePath->text());
    settings.setValue("moviesPath", ui->lineEdit_moviesPath->text());
    settings.setValue("lang", getLang(ui->comboBox_language->currentIndex()));
    this->close();
}

void SettingsWindow::on_buttonBox_rejected()
{
    this->close();
}

void SettingsWindow::on_pushButton_VLCPath_clicked()
{

    QString pathVlc = QFileDialog::getOpenFileName(this,"", ui->lineEdit_VLCPath->text(),"");
    if(pathVlc!="")
        ui->lineEdit_VLCPath->setText(pathVlc);
}

void SettingsWindow::on_pushButton_testPatternPath_clicked()
{
    QString pathTestPattern = QFileDialog::getOpenFileName(this,"", ui->lineEdit_testPatternPath->text(),"");
    if(pathTestPattern!="")
        ui->lineEdit_testPatternPath->setText(pathTestPattern);
}

void SettingsWindow::on_pushButton_intertitlePath_clicked()
{
    QString pathIntertitle = QFileDialog::getOpenFileName(this,"", ui->lineEdit_intertitlePath->text(),"");
    if(pathIntertitle!="")
        ui->lineEdit_intertitlePath->setText(pathIntertitle);
}

void SettingsWindow::on_pushButton_moviesPath_clicked()
{
    QString pathMovies = QFileDialog::getExistingDirectory(this, tr("Open Directory"), ui->lineEdit_moviesPath->text(),QFileDialog::ShowDirsOnly);
    if(pathMovies!="")
        ui->lineEdit_moviesPath->setText(pathMovies);
}

void SettingsWindow::on_radioButton_locateLeft_clicked()
{
    ui->radioButton_locateRight->setChecked(false);
    ui->radioButton_locateLeft->setChecked(true);
    std::stringstream ss;
        ss << (QApplication::desktop()->screen()->width()-QApplication::desktop()->screenGeometry().width());
    std::string sizeScreen ="screen-width="+ ss.str();
    _mediaPlayer->setSizeScreen(sizeScreen);
    _mediaPlayer->initStream();
}

void SettingsWindow::on_radioButton_locateRight_clicked()
{
    ui->radioButton_locateRight->setChecked(true);
    ui->radioButton_locateLeft->setChecked(false);
    std::stringstream ss;
        ss << QApplication::desktop()->screenGeometry().width();
    std::string sizeScreen ="screen-left="+ ss.str();
    _mediaPlayer->setSizeScreen(sizeScreen);

    _mediaPlayer->initStream();
}
