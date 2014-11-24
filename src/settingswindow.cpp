/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Cyril Naud <futuramath@gmail.com>
 *          Geoffrey Bergé <geoffrey.berge@live.fr>
 *          Thomas Berthomé <thoberthome@laposte.net>
 *          Thibaud Lamarche <lamarchethibaud@hotmail.com>
 *          Denis Saunier <saunier.denis.86@gmail.com>
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
#include "mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QSettings>
#include <QString>
#include <QFileDialog>
#include <QDir>
#include <QProcess>
#include <QMessageBox>

#include <string.h>
#include <sstream>

#include"mainwindow.h"
#include "utils.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    /**
     * default
     */
    QSettings settings("opp","opp");
    settings.setValue("openLastUsedListing", true);

    init();
}

void SettingsWindow::init(){
    /*Settings*/
    QSettings settings("opp","opp");
    ui->lineEdit_moviesPath->setText(settings.value("moviesPath").toString());
    ui->lineEdit_UpdatePath->setText(settings.value("updatePath").toString());
    ui->groupBox_3->setEnabled(false);
    setVideoReturnMode();

    ui->radioButton_locateRight->setChecked(settings.value("locateR").toBool());
    ui->radioButton_locateLeft->setChecked(!settings.value("locateR").toBool());

    ui->comboBox_language->setCurrentIndex(getIndex(settings.value("lang").toString()));
    QStringList sl;
    sl << "White" << "Yellow";
    ui->comboBox_SubtitleColor->addItems(sl);
    ui->comboBox_SubtitleColor->setCurrentIndex(settings.value("subtitleColor").toInt());

    if(settings.value("openLastUsedListing").toBool()){
        ui->openLastUsedListing->setChecked(true);
    }
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::setVideoReturnMode(){
    QSettings settings("opp","opp");
    if(settings.value("VideoReturnMode").toString() == "none"){
        ui->radioButton_None->setChecked(true);
        ui->radioButton_Pictures->setChecked(false);
        ui->radioButton_Streaming->setChecked(false);
    }else  if(settings.value("VideoReturnMode").toString() == "pictures"){
        ui->radioButton_Pictures->setChecked(true);
        ui->radioButton_Streaming->setChecked(false);
        ui->radioButton_None->setChecked(false);
    }else {
        ui->radioButton_Streaming->setChecked(true);
        ui->radioButton_Pictures->setChecked(false);
        ui->radioButton_None->setChecked(false);
        ui->groupBox_3->setEnabled(true);
    }
}

void SettingsWindow::setSettingsVideoReturnMode(){
    QSettings settings("opp","opp");
    if(ui->radioButton_Streaming->isChecked()){
        settings.setValue("VideoReturnMode","streaming");
    }else if(ui->radioButton_Pictures->isChecked()){
        settings.setValue("VideoReturnMode","pictures");
    }else{
        settings.setValue("VideoReturnMode","none");
    }
}


int SettingsWindow::getIndex(QString lang)
{
    if(lang=="en")
        return 0;
    else if(lang=="fr")
        return 1;
    else
        return -1;
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
    accept();
    this->close();
}

void SettingsWindow::accept() {
    QSettings settings("opp","opp");
    settings.setValue("moviesPath", ui->lineEdit_moviesPath->text());
    settings.setValue("lang", getLang(ui->comboBox_language->currentIndex()));
    settings.setValue("locateR", ui->radioButton_locateRight->isChecked());
    settings.setValue("updatePath", ui->lineEdit_UpdatePath->text());
    settings.setValue("subtitleColor", ui->comboBox_SubtitleColor->currentIndex());
    setSettingsVideoReturnMode();
}

void SettingsWindow::on_buttonBox_rejected()
{
    init();
    this->close();
}

void SettingsWindow::on_pushButton_moviesPath_clicked()
{
    QString pathMovies = QFileDialog::getExistingDirectory(this, tr("Open Directory"), ui->lineEdit_moviesPath->text(),QFileDialog::ShowDirsOnly);
    if(pathMovies!="")
        ui->lineEdit_moviesPath->setText(pathMovies);
}

void SettingsWindow::on_radioButton_Streaming_clicked()
{
    ui->groupBox_3->setEnabled(true);
    QMessageBox::information(this, "Apply",tr("There is a 2 seconds delay."));
}

void SettingsWindow::on_radioButton_Pictures_clicked()
{
    ui->groupBox_3->setEnabled(false);
}

void SettingsWindow::on_radioButton_None_clicked()
{
    ui->groupBox_3->setEnabled(false);
}

void SettingsWindow::on_restart_clicked()
{
    accept();
    QSettings settings("opp","opp");

    ((MainWindow *)this->parent())->verifSave();
    if(((MainWindow *)this->parent())->filename().compare("") != 0 && settings.value("openLastUsedListing").toBool()) {
        QStringList* arguments = new QStringList();
        arguments->append(((MainWindow *)this->parent())->filename());
        QProcess::startDetached(QApplication::applicationFilePath(), *arguments);
    }
    else
        QProcess::startDetached(QApplication::applicationFilePath());
    exit(2);
}

void SettingsWindow::on_openLastUsedListing_clicked()
{
    QSettings settings("opp","opp");
    if(ui->openLastUsedListing->isChecked()){
        settings.setValue("openLastUsedListing", true);
    }else{
        settings.setValue("openLastUsedListing", false);
    }
}
