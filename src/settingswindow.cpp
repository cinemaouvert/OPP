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
#include <QMetaObject>
#include <QMetaEnum>

#include <string.h>
#include <sstream>

#include"mainwindow.h"
#include "utils.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    _mw = (MainWindow*) parent;
    listOfShortcuts = QStringList() <<"UP"<<"DOWN"<<"LEFT"<<"RIGHT"
                                    <<"A"<<"B"<<"C"<<"D"<<"E"<<"F"<<"G"<<"H"<<"I"<<"J"
                                    <<"K"<<"L"<<"M"<<"N"<<"O"<<"P"<<"Q"<<"R"<<"S"<<"T"
                                    <<"U"<<"V"<<"W"<<"X"<<"Y"<<"Z"
                                    <<"F1"<<"F2"<<"F3"<<"F4"<<"F5"<<"F6"
                                    <<"F7"<<"F8"<<"F9"<<"F10"<<"F11"<<"F12"
                                    <<"SPACE"<<"ALT"<<"CTRL"<<"SHIFT"
                                    <<"DELETE" <<"PGUP"<<"PGDOWN";




    /**
     * default
     */
    QSettings settings("opp","opp");
    settings.setValue("openLastUsedListing", true);


    init();
    initShortcuts();
    connectShortcuts();
    initShorcuctsSettings();
    resetShorcuctsSettingsDefault();
    if(settings.value("shortcuts").toMap().size()>0){
        loadShortcuts();
    }
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
    sl <<"White" <<"Yellow";
    ui->comboBox_SubtitleColor->addItems(sl);
    ui->comboBox_SubtitleColor->setCurrentIndex(settings.value("subtitleColor").toInt());

    if(settings.value("openLastUsedListing").toBool()){
        ui->openLastUsedListing->setChecked(true);
    }

    //Pour cacher le mode Picture
    ui->radioButton_Pictures->hide();

}

SettingsWindow::~SettingsWindow()
{
    disconnectShortcuts();
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
    applyShortcuts();
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
    ui->groupBox_3->setEnabled(false);
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

void SettingsWindow::on_playPauseDefaultButton_clicked(){
        ui->playPauseShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp("SPACE"))+1);
}

void SettingsWindow::on_stopDefaultButton_clicked(){
    ui->stopShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp("S"))+1);
}

void SettingsWindow::on_previousDefaultButton_clicked(){
        ui->previousVideoShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp("UP"))+1);
}

void SettingsWindow::on_nextDefaultButton_clicked(){
        ui->nextVideoShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp("DOWN"))+1);
}

void SettingsWindow::on_rewindDefaultButton_clicked(){
        ui->rewindShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp("LEFT"))+1);
}

void SettingsWindow::on_forwardDefaultButton_clicked(){
        ui->forwardShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp("RIGHT"))+1);
}

void SettingsWindow::on_lockDefaultButton_clicked(){
    ui->lockShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp("L"))+1);
}

void SettingsWindow::on_loopDefaultButton_clicked(){
    ui->loopShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp("P"))+1);
}

void SettingsWindow::on_switchVideoModesDefaultButton_clicked(){
    ui->switchVideoModeShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp("F1"))+1);
}

void SettingsWindow::on_defaultShortcutsButton_clicked(){
    on_playPauseDefaultButton_clicked();
    on_stopDefaultButton_clicked();
    on_nextDefaultButton_clicked();
    on_previousDefaultButton_clicked();
    on_rewindDefaultButton_clicked();
    on_forwardDefaultButton_clicked();
    on_lockDefaultButton_clicked();
    on_loopDefaultButton_clicked();
    on_switchVideoModesDefaultButton_clicked();
}

void SettingsWindow::initShorcuctsSettings(){

    ui->playPauseShortcut->addItem("-");
    ui->stopShortcut->addItem("-");
    ui->rewindShortcut->addItem("-");
    ui->forwardShortcut->addItem("-");
    ui->previousVideoShortcut->addItem("-");
    ui->nextVideoShortcut->addItem("-");
    ui->lockShortcut->addItem("-");
    ui->loopShortcut->addItem("-");
    ui->switchVideoModeShortcut->addItem("-");


    ui->playPauseShortcut->addItems(listOfShortcuts);
    ui->stopShortcut->addItems(listOfShortcuts);
    ui->rewindShortcut->addItems(listOfShortcuts);
    ui->forwardShortcut->addItems(listOfShortcuts);
    ui->previousVideoShortcut->addItems(listOfShortcuts);
    ui->nextVideoShortcut->addItems(listOfShortcuts);
    ui->lockShortcut->addItems(listOfShortcuts);
    ui->loopShortcut->addItems(listOfShortcuts);
    ui->switchVideoModeShortcut->addItems(listOfShortcuts);
}

void SettingsWindow::resetShorcuctsSettingsDefault(){
    ui->playPauseShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp("SPACE"))+1);
    ui->rewindShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp("LEFT"))+1);
    ui->forwardShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp("RIGHT"))+1);
    ui->previousVideoShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp("UP"))+1);
    ui->nextVideoShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp("DOWN"))+1);
    ui->stopShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp("S"))+1);
    ui->lockShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp("L"))+1);
    ui->loopShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp("P"))+1);
    ui->switchVideoModeShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp("F1"))+1);
}

void SettingsWindow::initShortcuts(){
    playPause_shortcut = new QShortcut(QKeySequence("space"), _mw->playerControlWidget());
    previous_shortcut = new QShortcut(QKeySequence("up"), _mw->playerControlWidget());
    next_shortcut = new QShortcut(QKeySequence("down"), _mw->playerControlWidget());
    rewind_shortcut = new QShortcut(QKeySequence("left"), _mw->playerControlWidget());
    forward_shortcut = new QShortcut(QKeySequence("right"), _mw->playerControlWidget());
    stop_shortcut = new QShortcut(QKeySequence("s"), _mw->playerControlWidget());
    loop_shortcut = new QShortcut(QKeySequence("p"), _mw->playerControlWidget());
    lock_shortcut = new QShortcut(QKeySequence("l"), _mw->playerControlWidget());
    switchVideoModes_shortcut = new QShortcut(QKeySequence("F1"), _mw);
}



void SettingsWindow::applyShortcuts(){

    //Apply shortcuts to settings attribute
    QMap<QString, QVariant> shortcuts;
    shortcuts.insert("playPause",QVariant(ui->playPauseShortcut->currentText()));
    shortcuts.insert("stop",QVariant(ui->stopShortcut->currentText()));
    shortcuts.insert("rewind",QVariant(ui->rewindShortcut->currentText()));
    shortcuts.insert("forward",QVariant(ui->forwardShortcut->currentText()));
    shortcuts.insert("previous",QVariant(ui->previousVideoShortcut->currentText()));
    shortcuts.insert("next",QVariant(ui->nextVideoShortcut->currentText()));
    shortcuts.insert("lock",QVariant(ui->lockShortcut->currentText()));
    shortcuts.insert("loop",QVariant(ui->loopShortcut->currentText()));
    shortcuts.insert("switchVideoModes",QVariant(ui->switchVideoModeShortcut->currentText()));

    QSettings settings("opp","opp");
    settings.setValue("shortcuts", QVariant(shortcuts));

    //Apply shortcuts to software
    loadShortcuts();
}

void SettingsWindow::loadShortcuts(){
    QSettings settings("opp","opp");

     //QSettings settings("opp","opp");
    QMap<QString,QVariant> shortcuts = settings.value("shortcuts").toMap();

    if(shortcuts.value("playPause").toString() == "-"){
        playPause_shortcut->setEnabled(false);
    } else {
        playPause_shortcut->setEnabled(true);
        playPause_shortcut->setKey(QKeySequence(shortcuts.value("playPause").toString()));
    }
    if(shortcuts.value("previous").toString() == "-"){
        previous_shortcut->setEnabled(false);
    } else {
        previous_shortcut->setEnabled(true);
        previous_shortcut->setKey(QKeySequence(shortcuts.value("previous").toString()));
    }
    if(shortcuts.value("next").toString() == "-"){
        next_shortcut->setEnabled(false);
    } else {
        next_shortcut->setEnabled(true);
        next_shortcut->setKey(QKeySequence(shortcuts.value("next").toString()));

    }
    if(shortcuts.value("rewind").toString() == "-"){
        rewind_shortcut->setEnabled(false);
    } else {
        rewind_shortcut->setEnabled(true);
        rewind_shortcut->setKey(QKeySequence(shortcuts.value("rewind").toString()));

    }
    if(shortcuts.value("forward").toString() == "-"){
        forward_shortcut->setEnabled(false);
    } else {
        forward_shortcut->setEnabled(true);
        forward_shortcut->setKey(QKeySequence(shortcuts.value("forward").toString()));

    }
    if(shortcuts.value("stop").toString() == "-"){
        stop_shortcut->setEnabled(false);
    } else {
        stop_shortcut->setEnabled(true);
        stop_shortcut->setKey(QKeySequence(shortcuts.value("stop").toString()));
    }
    if(shortcuts.value("loop").toString() == "-"){

        loop_shortcut->setEnabled(false);
    } else {
        loop_shortcut->setEnabled(true);
        loop_shortcut->setKey(QKeySequence(shortcuts.value("loop").toString()));

    }
    if(shortcuts.value("lock").toString() == "-"){
        lock_shortcut->setEnabled(false);
    } else {
        lock_shortcut->setEnabled(true);
        lock_shortcut->setKey(QKeySequence(shortcuts.value("lock").toString()));
    }
    if(shortcuts.value("switchVideoModes").toString() == "-"){
        lock_shortcut->setEnabled(false);
    } else {
        switchVideoModes_shortcut->setEnabled(true);
         switchVideoModes_shortcut->setKey(QKeySequence(shortcuts.value("switchVideoModes").toString()));
    }

    ui->playPauseShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp(shortcuts.value("playPause").toString()))+1);
    ui->rewindShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp(shortcuts.value("rewind").toString()))+1);
    ui->forwardShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp(shortcuts.value("forward").toString()))+1);
    ui->previousVideoShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp(shortcuts.value("previous").toString()))+1);
    ui->nextVideoShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp(shortcuts.value("next").toString()))+1);
    ui->stopShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp(shortcuts.value("stop").toString()))+1);
    ui->lockShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp(shortcuts.value("lock").toString()))+1);
    ui->loopShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp(shortcuts.value("loop").toString()))+1);
    ui->switchVideoModeShortcut->setCurrentIndex(listOfShortcuts.indexOf(QRegExp(shortcuts.value("switchVideoModes").toString()))+1);


}

void SettingsWindow::connectShortcuts(){
    connect(playPause_shortcut, SIGNAL(activated()), _mw->playerControlWidget(), SLOT(playPauseKeyboard()));
    connect(stop_shortcut, SIGNAL(activated()), _mw->playerControlWidget()->stopButton(), SLOT(click()));
    connect(loop_shortcut, SIGNAL(activated()), _mw->playerControlWidget()->loopButton(), SLOT(click()));
    connect(lock_shortcut, SIGNAL(activated()), _mw->playerControlWidget()->lockButton(), SLOT(click()));
    connect(previous_shortcut, SIGNAL(activated()), _mw->playerControlWidget()->previousButton(), SLOT(click()));
    connect(next_shortcut, SIGNAL(activated()), _mw->playerControlWidget()->nextButton(), SLOT(click()));
    connect(rewind_shortcut, SIGNAL(activated()), _mw->playerControlWidget()->rewindButton(), SLOT(click()));
    connect(forward_shortcut, SIGNAL(activated()), _mw->playerControlWidget()->forwardButton(), SLOT(click()));
    connect(switchVideoModes_shortcut, SIGNAL(activated()), _mw, SLOT(switchVideoMode()));

    connect(ui->playPauseShortcut, SIGNAL(currentIndexChanged(int)), this, SLOT(on_playPause_changed()));
    connect(ui->stopShortcut, SIGNAL(currentIndexChanged(int)), this, SLOT(on_stop_changed()));
    connect(ui->loopShortcut, SIGNAL(currentIndexChanged(int)),this, SLOT(on_loop_changed()));
    connect(ui->lockShortcut, SIGNAL(currentIndexChanged(int)), this, SLOT(on_lock_changed()));
    connect(ui->previousVideoShortcut, SIGNAL(currentIndexChanged(int)), this, SLOT(on_previous_changed()));
    connect(ui->nextVideoShortcut, SIGNAL(currentIndexChanged(int)), this, SLOT(on_next_changed()));
    connect(ui->rewindShortcut, SIGNAL(currentIndexChanged(int)), this, SLOT(on_rewind_changed()));
    connect(ui->forwardShortcut, SIGNAL(currentIndexChanged(int)), this, SLOT(on_forward_changed()));
    connect(ui->switchVideoModeShortcut, SIGNAL(currentIndexChanged(int)), this, SLOT(on_switchVideoModes_changed()));
}

void SettingsWindow::disconnectShortcuts(){
    disconnect(playPause_shortcut, SIGNAL(activated()), _mw->playerControlWidget(), SLOT(playPauseKeyboard()));
    disconnect(stop_shortcut, SIGNAL(activated()), _mw->playerControlWidget()->stopButton(), SLOT(click()));
    disconnect(loop_shortcut, SIGNAL(activated()), _mw->playerControlWidget()->loopButton(), SLOT(click()));
    disconnect(lock_shortcut, SIGNAL(activated()), _mw->playerControlWidget()->lockButton(), SLOT(click()));
    disconnect(previous_shortcut, SIGNAL(activated()), _mw->playerControlWidget()->previousButton(), SLOT(click()));
    disconnect(next_shortcut, SIGNAL(activated()), _mw->playerControlWidget()->nextButton(), SLOT(click()));
    disconnect(rewind_shortcut, SIGNAL(activated()), _mw->playerControlWidget()->rewindButton(), SLOT(click()));
    disconnect(forward_shortcut, SIGNAL(activated()), _mw->playerControlWidget()->forwardButton(), SLOT(click()));
    disconnect(switchVideoModes_shortcut, SIGNAL(activated()), _mw, SLOT(switchVideoMode()));

    disconnect(ui->playPauseShortcut, SIGNAL(currentIndexChanged(int)), this, SLOT(on_playPause_changed()));
    disconnect(ui->stopShortcut, SIGNAL(currentIndexChanged(int)), this, SLOT(on_stop_changed()));
    disconnect(ui->loopShortcut, SIGNAL(currentIndexChanged(int)),this, SLOT(on_loop_changed()));
    disconnect(ui->lockShortcut, SIGNAL(currentIndexChanged(int)), this, SLOT(on_lock_changed()));
    disconnect(ui->previousVideoShortcut, SIGNAL(currentIndexChanged(int)), this, SLOT(on_previous_changed()));
    disconnect(ui->nextVideoShortcut, SIGNAL(currentIndexChanged(int)), this, SLOT(on_next_changed()));
    disconnect(ui->rewindShortcut, SIGNAL(currentIndexChanged(int)), this, SLOT(on_rewind_changed()));
    disconnect(ui->forwardShortcut, SIGNAL(currentIndexChanged(int)), this, SLOT(on_forward_changed()));
    disconnect(ui->switchVideoModeShortcut, SIGNAL(currentIndexChanged(int)), this, SLOT(on_switchVideoModes_changed()));
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

void SettingsWindow::on_playPause_changed(){
    if(ui->playPauseShortcut->currentText() == ui->stopShortcut->currentText()){
        ui->stopShortcut->setCurrentIndex(0);
    }
    if(ui->playPauseShortcut->currentText() == ui->previousVideoShortcut->currentText()){
        ui->previousVideoShortcut->setCurrentIndex(0);
    }
    if(ui->playPauseShortcut->currentText() == ui->nextVideoShortcut->currentText()){
        ui->nextVideoShortcut->setCurrentIndex(0);
    }
    if(ui->playPauseShortcut->currentText() == ui->rewindShortcut->currentText()){
        ui->rewindShortcut->setCurrentIndex(0);
    }
    if(ui->playPauseShortcut->currentText() == ui->forwardShortcut->currentText()){
        ui->forwardShortcut->setCurrentIndex(0);
    }
    if(ui->playPauseShortcut->currentText() == ui->loopShortcut->currentText()){
        ui->loopShortcut->setCurrentIndex(0);
    }
    if(ui->playPauseShortcut->currentText() == ui->lockShortcut->currentText()){
        ui->lockShortcut->setCurrentIndex(0);
    }
    if(ui->playPauseShortcut->currentText() == ui->switchVideoModeShortcut->currentText()){
        ui->switchVideoModeShortcut->setCurrentIndex(0);
    }
}

void SettingsWindow::on_stop_changed(){
    if( ui->stopShortcut->currentText() == ui->playPauseShortcut->currentText()){
        ui->playPauseShortcut->setCurrentIndex(0);
    }
    if(ui->stopShortcut->currentText() == ui->previousVideoShortcut->currentText()){
        ui->previousVideoShortcut->setCurrentIndex(0);
    }
    if(ui->stopShortcut->currentText() == ui->nextVideoShortcut->currentText()){
        ui->nextVideoShortcut->setCurrentIndex(0);
    }
    if(ui->stopShortcut->currentText() == ui->rewindShortcut->currentText()){
        ui->rewindShortcut->setCurrentIndex(0);
    }
    if(ui->stopShortcut->currentText() == ui->forwardShortcut->currentText()){
        ui->forwardShortcut->setCurrentIndex(0);
    }
    if(ui->stopShortcut->currentText() == ui->loopShortcut->currentText()){
        ui->loopShortcut->setCurrentIndex(0);
    }
    if(ui->stopShortcut->currentText() == ui->lockShortcut->currentText()){
        ui->lockShortcut->setCurrentIndex(0);
    }
    if(ui->stopShortcut->currentText() == ui->switchVideoModeShortcut->currentText()){
        ui->switchVideoModeShortcut->setCurrentIndex(0);
    }
}

void SettingsWindow::on_previous_changed(){
    if(ui->previousVideoShortcut->currentText() == ui->playPauseShortcut->currentText()){
        ui->playPauseShortcut->setCurrentIndex(0);
    }
    if(ui->previousVideoShortcut->currentText() == ui->stopShortcut->currentText()){
        ui->stopShortcut->setCurrentIndex(0);
    }
    if(ui->previousVideoShortcut->currentText() == ui->nextVideoShortcut->currentText()){
        ui->nextVideoShortcut->setCurrentIndex(0);
    }
    if(ui->previousVideoShortcut->currentText() == ui->rewindShortcut->currentText()){
        ui->rewindShortcut->setCurrentIndex(0);
    }
    if(ui->previousVideoShortcut->currentText() == ui->forwardShortcut->currentText()){
        ui->forwardShortcut->setCurrentIndex(0);
    }
    if(ui->previousVideoShortcut->currentText() == ui->loopShortcut->currentText()){
        ui->loopShortcut->setCurrentIndex(0);
    }
    if(ui->previousVideoShortcut->currentText() == ui->lockShortcut->currentText()){
        ui->lockShortcut->setCurrentIndex(0);
    }
    if(ui->previousVideoShortcut->currentText() == ui->switchVideoModeShortcut->currentText()){
        ui->switchVideoModeShortcut->setCurrentIndex(0);
    }
}

void SettingsWindow::on_next_changed(){
    if(ui->nextVideoShortcut->currentText() == ui->playPauseShortcut->currentText()){
        ui->playPauseShortcut->setCurrentIndex(0);
    }
    if(ui->nextVideoShortcut->currentText() == ui->stopShortcut->currentText()){
        ui->stopShortcut->setCurrentIndex(0);
    }
    if(ui->nextVideoShortcut->currentText() == ui->previousVideoShortcut->currentText()){
        ui->previousVideoShortcut->setCurrentIndex(0);
    }
    if(ui->nextVideoShortcut->currentText() == ui->rewindShortcut->currentText()){
        ui->rewindShortcut->setCurrentIndex(0);
    }
    if(ui->nextVideoShortcut->currentText() == ui->forwardShortcut->currentText()){
        ui->forwardShortcut->setCurrentIndex(0);
    }
    if(ui->nextVideoShortcut->currentText() == ui->loopShortcut->currentText()){
        ui->loopShortcut->setCurrentIndex(0);
    }
    if(ui->nextVideoShortcut->currentText() == ui->lockShortcut->currentText()){
        ui->lockShortcut->setCurrentIndex(0);
    }
    if(ui->nextVideoShortcut->currentText() == ui->switchVideoModeShortcut->currentText()){
        ui->switchVideoModeShortcut->setCurrentIndex(0);
    }
}

void SettingsWindow::on_rewind_changed(){
    if(ui->rewindShortcut->currentText() == ui->playPauseShortcut->currentText()){
        ui->playPauseShortcut->setCurrentIndex(0);
    }
    if(ui->rewindShortcut->currentText() == ui->stopShortcut->currentText()){
        ui->stopShortcut->setCurrentIndex(0);
    }
    if(ui->rewindShortcut->currentText() == ui->previousVideoShortcut->currentText()){
        ui->previousVideoShortcut->setCurrentIndex(0);
    }
    if(ui->rewindShortcut->currentText() == ui->nextVideoShortcut->currentText()){
        ui->nextVideoShortcut->setCurrentIndex(0);
    }
    if(ui->rewindShortcut->currentText() == ui->forwardShortcut->currentText()){
        ui->forwardShortcut->setCurrentIndex(0);
    }
    if(ui->rewindShortcut->currentText() == ui->loopShortcut->currentText()){
        ui->loopShortcut->setCurrentIndex(0);
    }
    if(ui->rewindShortcut->currentText() == ui->lockShortcut->currentText()){
        ui->lockShortcut->setCurrentIndex(0);
    }
    if(ui->rewindShortcut->currentText() == ui->switchVideoModeShortcut->currentText()){
        ui->switchVideoModeShortcut->setCurrentIndex(0);
    }
}

void SettingsWindow::on_forward_changed(){
    if(ui->forwardShortcut->currentText() == ui->playPauseShortcut->currentText()){
        ui->playPauseShortcut->setCurrentIndex(0);
    }
    if(ui->forwardShortcut->currentText() == ui->stopShortcut->currentText()){
        ui->stopShortcut->setCurrentIndex(0);
    }
    if(ui->forwardShortcut->currentText() == ui->previousVideoShortcut->currentText()){
        ui->previousVideoShortcut->setCurrentIndex(0);
    }
    if(ui->forwardShortcut->currentText() == ui->nextVideoShortcut->currentText()){
        ui->nextVideoShortcut->setCurrentIndex(0);
    }
    if(ui->forwardShortcut->currentText() == ui->rewindShortcut->currentText()){
        ui->rewindShortcut->setCurrentIndex(0);
    }
    if(ui->forwardShortcut->currentText() == ui->loopShortcut->currentText()){
        ui->loopShortcut->setCurrentIndex(0);
    }
    if(ui->forwardShortcut->currentText() == ui->lockShortcut->currentText()){
        ui->lockShortcut->setCurrentIndex(0);
    }
    if(ui->forwardShortcut->currentText() == ui->switchVideoModeShortcut->currentText()){
        ui->switchVideoModeShortcut->setCurrentIndex(0);
    }
}

void SettingsWindow::on_loop_changed(){
    if(ui->loopShortcut->currentText() == ui->playPauseShortcut->currentText()){
        ui->playPauseShortcut->setCurrentIndex(0);
    }
    if(ui->loopShortcut->currentText() == ui->stopShortcut->currentText()){
        ui->stopShortcut->setCurrentIndex(0);
    }
    if(ui->loopShortcut->currentText() == ui->previousVideoShortcut->currentText()){
        ui->previousVideoShortcut->setCurrentIndex(0);
    }
    if(ui->loopShortcut->currentText() == ui->nextVideoShortcut->currentText()){
        ui->nextVideoShortcut->setCurrentIndex(0);
    }
    if(ui->loopShortcut->currentText() == ui->rewindShortcut->currentText()){
        ui->rewindShortcut->setCurrentIndex(0);
    }
    if(ui->loopShortcut->currentText() == ui->forwardShortcut->currentText()){
        ui->forwardShortcut->setCurrentIndex(0);
    }
    if(ui->loopShortcut->currentText() == ui->lockShortcut->currentText()){
        ui->lockShortcut->setCurrentIndex(0);
    }
    if(ui->loopShortcut->currentText() == ui->switchVideoModeShortcut->currentText()){
        ui->switchVideoModeShortcut->setCurrentIndex(0);
    }
}

void SettingsWindow::on_lock_changed(){
    if(ui->lockShortcut->currentText() == ui->playPauseShortcut->currentText()){
        ui->playPauseShortcut->setCurrentIndex(0);
    }
    if(ui->lockShortcut->currentText() == ui->stopShortcut->currentText()){
        ui->stopShortcut->setCurrentIndex(0);
    }
    if(ui->lockShortcut->currentText() == ui->previousVideoShortcut->currentText()){
        ui->previousVideoShortcut->setCurrentIndex(0);
    }
    if(ui->lockShortcut->currentText() == ui->nextVideoShortcut->currentText()){
        ui->nextVideoShortcut->setCurrentIndex(0);
    }
    if(ui->lockShortcut->currentText() == ui->rewindShortcut->currentText()){
        ui->rewindShortcut->setCurrentIndex(0);
    }
    if(ui->lockShortcut->currentText() == ui->forwardShortcut->currentText()){
        ui->forwardShortcut->setCurrentIndex(0);
    }
    if(ui->lockShortcut->currentText() == ui->loopShortcut->currentText()){
        ui->loopShortcut->setCurrentIndex(0);
    }
    if(ui->lockShortcut->currentText() == ui->switchVideoModeShortcut->currentText()){
        ui->switchVideoModeShortcut->setCurrentIndex(0);
    }
}

void SettingsWindow::on_switchVideoModes_changed(){
    if(ui->switchVideoModeShortcut->currentText() == ui->playPauseShortcut->currentText()){
        ui->playPauseShortcut->setCurrentIndex(0);
    }
    if(ui->switchVideoModeShortcut->currentText() == ui->stopShortcut->currentText()){
        ui->stopShortcut->setCurrentIndex(0);
    }
    if(ui->switchVideoModeShortcut->currentText() == ui->previousVideoShortcut->currentText()){
        ui->previousVideoShortcut->setCurrentIndex(0);
    }
    if(ui->switchVideoModeShortcut->currentText() == ui->nextVideoShortcut->currentText()){
        ui->nextVideoShortcut->setCurrentIndex(0);
    }
    if(ui->switchVideoModeShortcut->currentText() == ui->rewindShortcut->currentText()){
        ui->rewindShortcut->setCurrentIndex(0);
    }
    if(ui->switchVideoModeShortcut->currentText() == ui->forwardShortcut->currentText()){
        ui->forwardShortcut->setCurrentIndex(0);
    }
    if(ui->switchVideoModeShortcut->currentText() == ui->loopShortcut->currentText()){
        ui->loopShortcut->setCurrentIndex(0);
    }
    if(ui->switchVideoModeShortcut->currentText() == ui->lockShortcut->currentText()){
        ui->lockShortcut->setCurrentIndex(0);
    }
}
