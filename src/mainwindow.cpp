/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
 *          Baptiste Rozière <bapt.roziere@gmail.com>
 *          Hamza Haddar <ham.haddar@gmail.com>
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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QHeaderView>
#include <QContextMenuEvent>
#include <QInputDialog>
#include <QPixmap>
#include <QSettings>
#include <QApplication>
#include <QPrinter>
#include <QPainter>
#include <QSysInfo>


#include <iostream>

#include "videowindow.h"
#include "settingswindow.h"
#include "advancedsettingswindow.h"
#include "advancedpicturesettingswindow.h"
#include "locksettingswindow.h"
#include "medialistmodel.h"
#include "mediatableview.h"
#include "playlisttableview.h"
#include "playlistmodel.h"
#include "schedulelistmodel.h"
#include "locker.h"
#include "statuswidget.h"
#include "updater.h"

#include "application.h"
#include "media.h"
#include "playlistplayer.h"
#include "mediaplayer.h"
#include "playback.h"
#include "utils.h"
#include "datastorage.h"
#include "aboutdialog.h"
#include "exportpdf.h"


#include "plugins.h"
#include <QPluginLoader>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _videoWindow(NULL),
    _settingsWindow(NULL),
    _lockSettingsWindow(NULL),
    _advancedSettingsWindow(NULL),
    _advancedPictureSettingsWindow(NULL),
    _aboutdialog(NULL),
    _statusWidget(NULL),
    _app(NULL),
    _playlistPlayer(NULL),
    _mediaListModel(NULL),
    _scheduleListModel(NULL),
    _locker(NULL) ,
    _dataStorage(NULL),
    _fileName(""),
    _projectionMode(VideoWindow::WINDOW),
    _selectedMediaName(NULL),
    _ocpmPlugin(NULL),
    _exportPDF(NULL)

{
    //setAttribute(Qt::WA_DeleteOnClose);

    ui->setupUi(this);

    _lastSelectedTab=-1;
    _locker = new Locker(getLockedWidget(), this);

    connect(_locker, SIGNAL(toggled(bool)), ui->lockButton, SLOT(setChecked(bool)));
    connect(ui->lockButton, SIGNAL(clicked(bool)), _locker, SLOT(toggle(bool)));

    _lockSettingsWindow = new LockSettingsWindow(_locker, this);

    // internal core initalization
    _app = new Application();
    _playlistPlayer = new PlaylistPlayer(_app->vlcInstance(), this);

    _videoWindow = new VideoWindow(this);

    _playlistPlayer->mediaPlayer()->setVideoView( (VideoView*) _videoWindow->videoWidget() );
    _playlistPlayer->mediaPlayer()->setVideoBackView( (VideoView*) ui->backWidget );
    _playlistPlayer->mediaPlayer()->initStream();
    _playlistPlayer->mediaPlayer()->setVolume(ui->playerVolumeSlider->value());

    connect(ui->playerVolumeSlider, SIGNAL(valueChanged(int)), _playlistPlayer->mediaPlayer(), SLOT(setVolume(int)));
    connect(_playlistPlayer->mediaPlayer(), SIGNAL(playing(bool)), ui->playerPlayButton, SLOT(setChecked(bool)));
    connect(ui->playerStopButton, SIGNAL(clicked()), this, SLOT(stop()));
    connect(ui->playerPreviousButton, SIGNAL(clicked()), _playlistPlayer, SLOT(previous()));
    connect(ui->playerNextButton, SIGNAL(clicked()), _playlistPlayer, SLOT(next()));
    connect(_playlistPlayer, SIGNAL(end()), ui->playerPlayButton, SLOT(toggle()));
    connect(ui->disableButton, SIGNAL(clicked()), this, SLOT(on_disableButton_clicked()));

    _mediaListModel = new MediaListModel();
    _scheduleListModel = new ScheduleListModel();

    connect(ui->scheduleToggleEnabledButton, SIGNAL(toggled(bool)), _scheduleListModel, SLOT(toggleAutomation(bool)));

    ui->binTableView->setModel(_mediaListModel);
    ui->scheduleTableView->setModel(_scheduleListModel);
    ui->seekWidget->setMediaPlayer(_playlistPlayer->mediaPlayer(), this);

    connect(_mediaListModel, SIGNAL(mediaListChanged(int)),this, SLOT(updateProjectSummary()));

    connect(_scheduleListModel, SIGNAL(scheduleListChanged()),this, SLOT(updateProjectSummary()));

    _statusWidget = new StatusWidget;
    ui->statusBar->addWidget(_statusWidget);
    connect(_mediaListModel, SIGNAL(mediaListChanged(int)), _statusWidget, SLOT(setMediaCount(int)));

    // show/hide pannel actions
    //connect(ui->quitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->resumeDetailsAction, SIGNAL(toggled(bool)), ui->projectTabWidget, SLOT(setVisible(bool)));
    connect(ui->binAction, SIGNAL(toggled(bool)), ui->binGroupBox, SLOT(setVisible(bool)));
    connect(ui->automationAction, SIGNAL(toggled(bool)), ui->scheduleGroupBox, SLOT(setVisible(bool)));
    connect(ui->statusBarAction, SIGNAL(toggled(bool)), ui->statusBar, SLOT(setVisible(bool)));

    connect(ui->newPlaylistAction, SIGNAL(triggered()), this, SLOT(on_addPlaylistButton_clicked()));

    // set video mode actions data
    ui->actionProjection->setData(QVariant(VideoWindow::PROJECTION));
    ui->actionWindow->setData(QVariant(VideoWindow::WINDOW));

    _dataStorage = new DataStorage(_app, this);
    _dataStorage->setMediaListModel(_mediaListModel);
    _dataStorage->setScheduleListModel(_scheduleListModel);

    connect(ui->progEdit,SIGNAL(textChanged(QString)), _dataStorage, SLOT(setProjectTitle(QString)));

    createPlaylistTab();

    ui->scheduleLaunchAtDateEdit->setDate(QDate::currentDate());
    ui->scheduleLaunchAtTimeEdit->setTime(QTime::currentTime());

    ui->ratioComboBox->clear();
    ui->ratioComboBox->addItems(MediaSettings::ratioValues());

    ui->subtitlesEncodecomboBox->clear();
    ui->subtitlesEncodecomboBox->addItems(MediaSettings::encodeValues());

    _advancedSettingsWindow = new AdvancedSettings(this);
    _advancedPictureSettingsWindow = new AdvancedPictureSettingsWindow(this);
    _settingsWindow = new SettingsWindow(this);
    _exportPDF = new ExportPDF(this);

    QSettings settings("opp","opp");
    if(settings.value("VideoReturnMode").toString() == "pictures")
    {
        ui->stackedWidget->setCurrentIndex(1);
        _playlistPlayer->mediaPlayer()->setBackMode(MediaPlayer::SCREENSHOT);
    }
    else if(settings.value("VideoReturnMode").toString() == "streaming")
    {
        ui->stackedWidget->setCurrentIndex(0);
        _playlistPlayer->mediaPlayer()->setBackMode(MediaPlayer::STREAMING);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(2);
        _playlistPlayer->mediaPlayer()->setBackMode(MediaPlayer::NONE);
    }

    ui->scheduleToggleEnabledButton->click();

    currentPlaylistTableView()->setDragDropMode(QAbstractItemView::InternalMove);
    currentPlaylistTableView()->setDragEnabled(true);

    _aboutdialog = new AboutDialog();

    //Init selectedFileName
    _selectedMediaName = new QString("");

    ui->textEdit_Codecs->setReadOnly(1);
    ui->textEdit_Codecs->append("<span style=\"text-decoration: underline;\">Codecs:</span>");

    _logger = LoggerSingleton::getInstance();
    _logger->setTextEdit(ui->label_5);

    //Chargement des plugins
    loadPlugins();

    //Redimensionnement colonnes Bin
    ui->binTableView->setColumnWidth(1,80);
    ui->binTableView->setColumnWidth(2,60);

    //Restart
    if(QApplication::argc()>1) //Restart : filename en argument
        openFile(QApplication::arguments()[1]);
}

MainWindow::~MainWindow()
{
    if(ui != NULL)
        delete ui;
    if(_lockSettingsWindow != NULL)
        delete _lockSettingsWindow;
    if(_mediaListModel != NULL)
        delete _mediaListModel;
    if(_scheduleListModel != NULL)
        delete _scheduleListModel;
    if(_videoWindow != NULL)
        delete _videoWindow;
    if(_playlistPlayer != NULL)
        delete _playlistPlayer;
    if(_dataStorage != NULL)
        delete _dataStorage;
    if(_app != NULL)
        delete _app;
    if(_advancedSettingsWindow != NULL)
        delete _advancedSettingsWindow;
    if(_advancedPictureSettingsWindow != NULL)
        delete _advancedPictureSettingsWindow;
    if(_settingsWindow != NULL)
        delete _settingsWindow;
    if(_aboutdialog != NULL)
        delete _aboutdialog;
    if(_statusWidget != NULL)
        delete _statusWidget;
    if(_locker != NULL)
        delete _locker;
    LoggerSingleton::destroyInstance();
}

// FIX : ref 0000001
QTabWidget* MainWindow::playlistTabWidget() const { return ui->playlistsTabWidget; }

Locker* MainWindow::getLocker() {
    return _locker;
}

QString MainWindow::getFilename() {
    return _fileName;
}

void MainWindow::openFile(QString fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite)) {
        QMessageBox::information(this, tr("Unable to open file"),file.errorString());
    }
    _fileName = fileName;
    _dataStorage->load(file);
    updatePlaylistListCombox();
    file.close();

    ui->progEdit->setText(_dataStorage->projectTitle());
    ui->notesEdit->setText(_dataStorage->projectNotes());
}

/***********************************************************************************************\
                                          Project Details/Summary
\***********************************************************************************************/

void MainWindow::updateProjectSummary()
{
    ui->countMoviesLabel->setText( QString::number(_mediaListModel->countMovies()) );
    ui->countPicturesLabel->setText( QString::number(_mediaListModel->countPictures()) );
    //ui->totalDurationLabel->setText( _mediaListModel->totalDuration().toString("hh:mm:ss") );
    ui->totalDurationLabel->setText( _scheduleListModel->totalDuration().toString("hh:mm:ss") );
}

void MainWindow::updateDetails() {
    ui->textEdit_Codecs->clear();
    ui->textEdit_Codecs->append("<span style=\"text-decoration: underline;\">Codecs:</span>");
    for(int i=0; i<ui->playlistsTabWidget->count();i++) {
        PlaylistModel *model = (PlaylistModel*) ((PlaylistTableView*) ui->playlistsTabWidget->widget(i))->model();
        foreach(Playback *playback, model->playlist()->playbackList()) {
            foreach(AudioTrack audioTrack, playback->media()->audioTracks()) {
                QString codec = audioTrack.codecDescription();
                if(!ui->textEdit_Codecs->toHtml().contains(codec))
                    ui->textEdit_Codecs->append(codec);
            }
        }
    }
}

void MainWindow::on_notesEdit_textChanged()
{
    _dataStorage->setProjectNotes(ui->notesEdit->document()->toPlainText());
}


/***********************************************************************************************\
                                          Media List Management
\***********************************************************************************************/

void MainWindow::on_binAddMediaButton_clicked()
{
    QSettings settings("opp", "opp");

    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("New media"), settings.value("moviesPath").toString(), tr("Media (%1)").arg(Media::mediaExtensions().join(" ")));

    libvlc_instance_t *vlc = libvlc_new(0, NULL);
    libvlc_media_player_t *vlcMP = libvlc_media_player_new(vlc);

    if(!QDir("screenshot").exists())
    {
        QDir().mkdir("screenshot");
    }

    foreach (QString fileName, fileNames) {
        Media *media = new Media(fileName, _app->vlcInstance());

        QString screenPath = "./screenshot/";
        screenPath = screenPath.replace("/",QDir::separator());
        screenPath +=  media->getLocation().replace(QDir::separator(),"_").remove(":");
        screenPath += ".png";

        libvlc_video_set_scale (vlcMP, 0.5f);
        if(!QFile(screenPath).exists() && !media->isAudio() && !media->isImage())
        {

            libvlc_media_player_set_media(vlcMP, media->core());
            libvlc_media_add_option(media->core(),":noaudio");
            libvlc_media_player_play(vlcMP);
            libvlc_media_player_set_position(vlcMP, 0.5f);
            float width = libvlc_video_get_width(vlcMP);
            float height = libvlc_video_get_height(vlcMP);
            waitSnap(750);
            libvlc_video_take_snapshot(vlcMP, 0, screenPath.toStdString().c_str(), width,height);
            waitSnap(100);

        }
        libvlc_media_player_stop(vlcMP);


        if (media->exists() == false) {
            QMessageBox::warning(this, tr("Import media"), QString(tr("The file %1 does not exist. Maybe it was deleted.")).arg(media->location()));
            delete media;
        } else if (_mediaListModel->addMedia(media) == false) {
            QMessageBox::warning(this, tr("Import media"), QString(tr("The file %1 was already imported or has a null timecode.")).arg(media->location()));
            delete media;
        }
    }

    libvlc_media_player_release(vlcMP);
    libvlc_vlm_release(vlc);


}



void MainWindow::on_binDeleteMediaButton_clicked()
{
    QItemSelectionModel *selectionModel = ui->binTableView->selectionModel();
    //TODO check if working well
    int nbMedia = selectionModel->selectedRows().length();
    for(int i=0;i<nbMedia;i++){
        bool toDel = true;

        QModelIndex index = selectionModel->selectedRows().at(i);
        Media *media = _mediaListModel->mediaList().at(index.row());

        if (media->isUsed()) {
            //If user answer Yes
            if (1 == QMessageBox::warning(this, media->name(), tr("This media is used. All references of this media into playlists will be deleted too.\n Are you sure to remove this media ?") ,tr("No"), tr("Yes")))
            {
                int countPlaylists = ui->playlistsTabWidget->count();
                for (int i = 0; i < countPlaylists; i++) {
                    PlaylistModel *model = (PlaylistModel*) ((PlaylistTableView*) ui->playlistsTabWidget->widget(i))->model();
                    if(!model->isRunningMedia(media)){
                        model->removePlaybackWithDeps(media);
                        updateSettings();

                    }else{
                        toDel=false;
                        QMessageBox::critical(this, media->name(), tr("The media wasn't removed because you can not delete files that have been or are  being used.") ,tr("OK"));
                    }
                }
            }else{
                toDel = false;
            }
        }
        if(toDel){
            _mediaListModel->removeMedia(index.row());
            // We removed a line so we need to update nbLine
            nbMedia--;
            i--;
        }
    }
}

/***********************************************************************************************\
                                     Playback Settings Management
\***********************************************************************************************/

void MainWindow::on_disableButton_clicked()
{
    if(!ui->disableButton->isChecked())
    {
        ui->disableButton->setText(tr("Disable"));
        _playlistPlayer->mediaPlayer()->setBackMode(MediaPlayer::STREAMING);
        if(_playlistPlayer->isPlaying())
        {
            _playlistPlayer->mediaPlayer()->playStream();
        }
    }
    else
    {
        ui->disableButton->setText(tr("Enable"));
        _playlistPlayer->mediaPlayer()->setBackMode(MediaPlayer::NONE);
        if(_playlistPlayer->isPlaying())
        {
            _playlistPlayer->mediaPlayer()->stopStream();
        }
    }
}


void MainWindow::on_advancedSettingsButton_clicked()
{
    _advancedSettingsWindow->setPlayback(selectedPlayback());
    _advancedSettingsWindow->show();
    _advancedSettingsWindow->raise();
    _advancedSettingsWindow->activateWindow();
}

void MainWindow::on_advancedPictureSettingsButton_clicked()
{
    _advancedPictureSettingsWindow->setPlayback(selectedPlayback());
    _advancedPictureSettingsWindow->show();
    _advancedPictureSettingsWindow->raise();
    _advancedPictureSettingsWindow->activateWindow();
}

void MainWindow::on_settingsAction_triggered()
{
    _settingsWindow->show();
    _settingsWindow->raise();
    _settingsWindow->activateWindow();
}

void MainWindow::on_lockSettingsAction_triggered()
{
    _lockSettingsWindow->setLocker();
    _lockSettingsWindow->show();
    _lockSettingsWindow->raise();
    _lockSettingsWindow->activateWindow();
}

void MainWindow::on_audioTrackComboBox_currentIndexChanged(int index)
{
    if (index == -1)
        return;

    Playback *playback = selectedPlayback();

    if (playback) {
        playback->mediaSettings()->setAudioTrack(index);
    }

    currentPlaylistModel()->updateLayout();
}

void MainWindow::on_videoTrackComboBox_currentIndexChanged(int index)
{
    if (index == -1)
        return;

    Playback *playback = selectedPlayback();

    if (playback) {
        playback->mediaSettings()->setVideoTrack(index == 0 ? -1 : index-1);
    }

    currentPlaylistModel()->updateLayout();
}

void MainWindow::on_subtitlesTrackComboBox_currentIndexChanged(int index)
{
    if (index == -1)
        return;

    Playback *playback = selectedPlayback();

    if (playback) {
        playback->mediaSettings()->setSubtitlesTrack(index);
    }

    currentPlaylistModel()->updateLayout();
}

void MainWindow::on_ratioComboBox_currentIndexChanged(int index)
{
    Playback *playback = selectedPlayback();
    if (playback) {
        playback->mediaSettings()->setRatio((Ratio) index);
    }
}

void MainWindow::on_subtitlesSyncSpinBox_valueChanged(double arg1)
{
    Playback *playback = selectedPlayback();
    if (playback) {
        playback->mediaSettings()->setSubtitlesSync(arg1);
    }
}

void MainWindow::on_gammaSpinBox_valueChanged(double arg1)
{
    Playback *playback = selectedPlayback();
    if (playback) {
        playback->mediaSettings()->setGamma(arg1);
    }
}

void MainWindow::on_contrastSpinBox_valueChanged(double arg1)
{
    Playback *playback = selectedPlayback();
    if (playback) {
        playback->mediaSettings()->setContrast(arg1);
    }
}

void MainWindow::on_brightnessSpinBox_valueChanged(double arg1)
{
    Playback *playback = selectedPlayback();
    if (playback) {
        playback->mediaSettings()->setBrightness(arg1);
    }
}

void MainWindow::on_saturationSpinBox_valueChanged(double arg1)
{
    Playback *playback = selectedPlayback();
    if (playback) {
        playback->mediaSettings()->setSaturation(arg1);
    }
}

void MainWindow::on_hueSpinBox_valueChanged(int arg1)
{
    Playback *playback = selectedPlayback();
    if (playback) {
        playback->mediaSettings()->setHue((int)arg1);
    }
}

void MainWindow::on_audioSyncDoubleSpinBox_valueChanged(double arg1)
{
    Playback *playback = selectedPlayback();
    if (playback) {
        playback->mediaSettings()->setAudioSync(arg1);
    }
}

void MainWindow::on_audioGainDoubleSpinBox_valueChanged(double arg1)
{
    Playback *playback = selectedPlayback();
    if (playback) {
        playback->mediaSettings()->setGain(arg1);
    }
}

void MainWindow::updateSettings()
{
    Playback* playback = selectedPlayback();
    if(!playback){
        ui->mediaSettingsWidget->setEnabled(false);
        ui->advancedSettingsButton->setEnabled(false);
        ui->advancedPictureSettingsButton->setEnabled(false);
        ui->playerControlsWidget->setEnabled(false);
        return;
    }

    ui->audioTrackComboBox->blockSignals(true);
    ui->videoTrackComboBox->blockSignals(true);
    ui->subtitlesTrackComboBox->blockSignals(true);

    ui->mediaSettingsWidget->setEnabled(true);
    ui->advancedSettingsButton->setEnabled(true);
    ui->advancedPictureSettingsButton->setEnabled(true);
    ui->playerControlsWidget->setEnabled(true);

    ui->audioTrackComboBox->clear();
    ui->audioTrackComboBox->addItem(tr("Disabled"));
    ui->audioTrackComboBox->addItems(playback->media()->audioTracksName());

    ui->videoTrackComboBox->clear();
    ui->videoTrackComboBox->addItem(tr("Disabled"));
    ui->videoTrackComboBox->addItems(playback->media()->videoTracksName());

    ui->subtitlesTrackComboBox->clear();
    ui->subtitlesTrackComboBox->addItem(tr("Disabled"));
    ui->subtitlesTrackComboBox->addItems(playback->media()->subtitlesTracksName());

    ui->subtitlesSyncSpinBox->setValue(playback->mediaSettings()->subtitlesSync());

    ui->gammaSpinBox->setValue(playback->mediaSettings()->gamma());
    ui->contrastSpinBox->setValue(playback->mediaSettings()->contrast());
    ui->brightnessSpinBox->setValue(playback->mediaSettings()->brightness());
    ui->saturationSpinBox->setValue(playback->mediaSettings()->saturation());
    ui->hueSpinBox->setValue(playback->mediaSettings()->hue());
    ui->audioGainDoubleSpinBox->setValue(playback->mediaSettings()->gain());

    const int track = playback->mediaSettings()->videoTrack();
    ui->videoTrackComboBox->setCurrentIndex( track == -1 ? 0 : track+1);

    ui->audioTrackComboBox->setCurrentIndex( playback->mediaSettings()->audioTrack() );
    ui->subtitlesTrackComboBox->setCurrentIndex( playback->mediaSettings()->subtitlesTrack() );

    ui->ratioComboBox->setCurrentIndex( playback->mediaSettings()->ratio() );

    //ui->subtitlesEncodecomboBox->clear();
    ui->subtitlesEncodecomboBox->setCurrentIndex( playback->mediaSettings()->subtitlesEncode() );

    ui->audioTrackComboBox->blockSignals(false);
    ui->videoTrackComboBox->blockSignals(false);
    ui->subtitlesTrackComboBox->blockSignals(false);

    ui->scheduleLaunchAtTimeEdit->setCurrentSectionIndex(1);
}


/***********************************************************************************************\
                                          Player
\***********************************************************************************************/


void MainWindow::on_playerPlayButton_clicked(bool checked)
{
    //UNUSED PlaylistModel *playlistModel = currentPlaylistModel();
    if (checked) {
        if (_playlistPlayer->mediaPlayer()->isPaused()) {
            _playlistPlayer->mediaPlayer()->resume();
        } else {
            //Creation de la window elle n'existe pas
            if(!_videoWindow->isVisible()){
                delete(_videoWindow);
                _videoWindow = new VideoWindow(this, _projectionMode);
                _playlistPlayer->mediaPlayer()->setVideoView( (VideoView*) _videoWindow->videoWidget() );
            }

            // play or resume playback

            QModelIndexList indexes = currentPlaylistTableView()->selectionModel()->selectedRows();


            // if no selected item play current playlist from first item
            if (indexes.count() == 0) {
                _playlistPlayer->playItemAt(0);
                // play playlist at selected item otherwise
            } else {
                const int index = indexes.first().row();
                _playlistPlayer->playItemAt(index);
            }
        }
    } else {
        _playlistPlayer->mediaPlayer()->pause();
    }
}

void MainWindow::on_menuVideoMode_triggered(QAction *action)
{
    foreach(QAction *otherAction, ui->menuVideoMode->actions()) {
        if (otherAction != action) {
            otherAction->setEnabled(true);
            otherAction->setChecked(false);
        } else {
            action->setEnabled(false);
            action->setChecked(true);
            _projectionMode =  (VideoWindow::DisplayMode) action->data().toInt() ;
            _videoWindow->setDisplayMode(_projectionMode);
        }
    }
}

// TODO : play test pattern
void MainWindow::on_testPatternAction_triggered()
{
}

void MainWindow::stop(){
    if(_playlistPlayer->mediaPlayer()->isPaused() || _playlistPlayer->mediaPlayer()->isPlaying())
        _playlistPlayer->stop();
    ui->playerPlayButton->setChecked(false);
}


/***********************************************************************************************\
                                          Playlist
\***********************************************************************************************/

void MainWindow::createPlaylistTab()
{
    createPlaylistTab("New Playlist");
}

void MainWindow::createPlaylistTab(QString name)
{
    if(_locker->isLock())
        QMessageBox::critical(this, tr("Add new playlist"), tr("The playlist is currently locked, you can not add a new playlist.") , tr("OK"));
    else {
        PlaylistTableView *newTab = new PlaylistTableView(this);
        Playlist *playlist = new Playlist(name);
        PlaylistModel *newModel = new PlaylistModel(playlist, _mediaListModel, _scheduleListModel);

        connect(playlist, SIGNAL(titleChanged()), _scheduleListModel, SIGNAL(layoutChanged()));
        connect(playlist, SIGNAL(playlistChanged()), this, SLOT(updateDetails()));

        newTab->setModel(newModel);
        newTab->setSelectionBehavior(QAbstractItemView::SelectRows);
        newTab->horizontalHeader()->setStretchLastSection(true);

        ui->playlistsTabWidget->addTab(newTab, playlist->title());
        ui->playlistsTabWidget->setCurrentWidget(newTab);

        updatePlaylistListCombox();
    }
}

void MainWindow::on_playlistsTabWidget_tabCloseRequested(int index)
{
    Playlist *playlist = playlistAt(index);

    PlaylistModel *model = (PlaylistModel*) ((PlaylistTableView*) ui->playlistsTabWidget->widget(index))->model();
    if(model->isRunning()){
        QMessageBox::critical(this, tr("Remove playlist"), tr("This playlist is currently running, you can't delete it.") , tr("OK"));
    }else{
        if (_scheduleListModel->isScheduled(playlist)) {
            if (0 == QMessageBox::warning(this, tr("Remove playlist"), tr("This playlist was scheduled. All schedules which use this playlist will be deleted too.\n Are you sure to remove this playlist ?") ,tr("No"), tr("Yes")))
                return;
            _scheduleListModel->removeScheduleWithDeps(playlist);
        }

        if (ui->playlistsTabWidget->count() == 1) {
            createPlaylistTab();
        }

        delete (PlaylistModel*) ((PlaylistTableView*) ui->playlistsTabWidget->widget(index))->model();
        ui->playlistsTabWidget->removeTab(index);

        updateSettings();
        updatePlaylistListCombox();
        updateDetails();

    }
}

void MainWindow::on_playlistsTabWidget_currentChanged(int index)
{

    if(_lastSelectedTab != -1){
        PlaylistTableView *view = (PlaylistTableView*) ui->playlistsTabWidget->widget(_lastSelectedTab);
        if(view != NULL){
            PlaylistModel *model = (PlaylistModel*) view->model();
            if(model != NULL){
                if(model->isRunning()){
                    return;
                }
                disconnect(_playlistPlayer->mediaPlayer(), SIGNAL(playing()), model, SLOT(playItem()));
                disconnect(_playlistPlayer->mediaPlayer(), SIGNAL(paused()), model, SLOT(pauseItem()));
                disconnect(_playlistPlayer->mediaPlayer(), SIGNAL(stopped()), model, SLOT(stopItem()));
                disconnect(_playlistPlayer, SIGNAL(itemChanged(int)), model, SLOT(setPlayingItem(int)));
            }
        }
    }

    PlaylistTableView *view = (PlaylistTableView*) ui->playlistsTabWidget->widget(index);
    PlaylistModel *model = (PlaylistModel*) view->model();

    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(updateSettings()));

    connect(_playlistPlayer->mediaPlayer(), SIGNAL(playing()), model, SLOT(playItem()));
    connect(_playlistPlayer->mediaPlayer(), SIGNAL(paused()), model, SLOT(pauseItem()));
    connect(_playlistPlayer->mediaPlayer(), SIGNAL(stopped()), model, SLOT(stopItem()));
    connect(_playlistPlayer, SIGNAL(itemChanged(int)), model, SLOT(setPlayingItem(int)));



    updateSettings();

    _playlistPlayer->setPlaylist(model->playlist());
    _lastSelectedTab = index;
}

void MainWindow::on_renamePlaylistAction_triggered()
{
    editPlaylistName();
}

void MainWindow::on_removePlaylistItemAction_triggered()
{
    deletePlaylistItem();

}

// FIX : ref 0000001
void MainWindow::restorePlaylistTab(PlaylistModel *model)
{
    PlaylistTableView *newTab = new PlaylistTableView(this);
    connect(model->playlist(), SIGNAL(titleChanged()), _scheduleListModel, SIGNAL(layoutChanged()));

    newTab->setModel(model);
    newTab->setSelectionBehavior(QAbstractItemView::SelectRows);
    newTab->horizontalHeader()->setStretchLastSection(true);

    ui->playlistsTabWidget->addTab(newTab, model->playlist()->title());
    ui->playlistsTabWidget->setCurrentWidget(newTab);

    updatePlaylistListCombox();

}

void MainWindow::on_playlistUpButton_clicked()
{
    QModelIndexList indexes = currentPlaylistTableView()->selectionModel()->selectedRows();
    if(indexes.count()==0)
        return;

    if(currentPlaylistModel()->moveUp(indexes.first())){
        currentPlaylistTableView()->setCurrentIndex(currentPlaylistModel()->index(indexes.first().row() - 1, indexes.first().column()));
        if( _playlistPlayer->getCurrentIndex() == ((QModelIndex)indexes.first()).row())
            _playlistPlayer->currentIndexDown();
        else if (_playlistPlayer->getCurrentIndex() == ((QModelIndex)indexes.first()).row()-1)
            _playlistPlayer->currentIndexUp();
    }
}

void MainWindow::on_playlistDownButton_clicked()
{
    QModelIndexList indexes = currentPlaylistTableView()->selectionModel()->selectedRows();

    if(indexes.count()==0)
        return;

    if(currentPlaylistModel()->moveDown(indexes.first())){
        currentPlaylistTableView()->setCurrentIndex(currentPlaylistModel()->index(indexes.first().row() + 1, indexes.first().column()));
        if( _playlistPlayer->getCurrentIndex() == ((QModelIndex)indexes.first()).row())
            _playlistPlayer->currentIndexUp();
        else if (_playlistPlayer->getCurrentIndex() == ((QModelIndex)indexes.first()).row()+1)
            _playlistPlayer->currentIndexDown();
    }
}

void MainWindow::on_addPlaylistButton_clicked()
{
    if(!_locker->isLock()) {
        bool ok;

        QString text = QInputDialog::getText(this,
                                             tr("New playlist"),
                                             tr("Playlist title : "),
                                             QLineEdit::Normal,
                                             tr("New playlist"),
                                             &ok
                                             );
        if(ok) {
            createPlaylistTab(text);
        }
    }
    else
        QMessageBox::critical(this, tr("Add new playlist"), tr("The playlist is currently locked, you can not add a new playlist.") , tr("OK"));

}

void MainWindow::on_editNamePlaylistButton_clicked()
{
    editPlaylistName();
}

void MainWindow::on_deletePlaylistItemButton_clicked()
{
    deletePlaylistItem();
    if(currentPlaylistTableView() != NULL && currentPlaylistTableView()->selectionModel() != NULL){
        QModelIndexList indexes = currentPlaylistTableView()->selectionModel()->selectedRows();
        if(indexes.count()>0)
            setSelectedMediaTimeByIndex(indexes.first().row());
    }
}

void MainWindow::editPlaylistName()
{
    if(_locker->isLock())
        QMessageBox::critical(this, tr("Edit playlist name"), tr("This playlist is currently locked, you can not edit the name of the playlist.") , tr("OK"));
    else {
        int tabIndex = ui->playlistsTabWidget->currentIndex();
        bool ok;

        QString text = QInputDialog::getText(this,
                                             tr("Rename playlist"),
                                             tr("Playlist title : "),
                                             QLineEdit::Normal,
                                             ui->playlistsTabWidget->tabText(tabIndex),
                                             &ok
                                             );

        if (ok && !text.isEmpty()) {
            ui->playlistsTabWidget->setTabText(tabIndex, text);
            currentPlaylistModel()->playlist()->setTitle(text);
            updatePlaylistListCombox();
        }
    }
}

void MainWindow::deletePlaylistItem()
{
    if(_locker->isLock())
        QMessageBox::critical(this, tr("Delete playlist item"), tr("This playlist is currently locked, you can not delete an item.") , tr("OK"));
    else {
        QModelIndexList indexes = currentPlaylistTableView()->selectionModel()->selectedRows();

        if (indexes.count() == 0)
            return;
        if(!currentPlaylistModel()->isRunningMedia(indexes.first().row())){
            currentPlaylistModel()->removePlayback(indexes.first().row());
            updateSettings();
            _scheduleListModel->updateLayout();
        }else{
            QMessageBox::critical(this, tr("Remove item"), tr("This playlist is currently running, you can not delete media that have been or are being displayed.") , tr("OK"));
        }
    }

}

/***********************************************************************************************\
                                          Project import/export
\***********************************************************************************************/

void MainWindow::verifSave () {
    if(_mediaListModel->rowCount()!=0)
        if (1 == QMessageBox::warning(this, tr("Save"), tr("Do you want to save the current listing ? \nOtherwise unsaved data will be lost.") ,tr("No"), tr("Yes")))
            on_saveAction_triggered();
}

void MainWindow::on_saveAction_triggered()
{
    if(_fileName.compare("")==0)
        on_saveAsAction_triggered();
    else {
        QFile file(_fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file."),file.errorString());
        }else{
            for (int i = 0; i < ui->playlistsTabWidget->count(); i++)
                _dataStorage->addPlaylistModel((PlaylistModel*) ( (PlaylistTableView*) ui->playlistsTabWidget->widget(i) )->model());

            _dataStorage->save(file);
            file.close();
            QMessageBox::information(this, tr("Saved"),tr("Listing saved."));
        }
    }
}

void MainWindow::on_saveAsAction_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Listing"), "", tr("OPP file (*.opp)"));

    if (fileName.isEmpty()) {
        return;
    } else {
        if(fileName.right(4)!=".opp")
            fileName+=".opp";
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file."),file.errorString());
        }else{
            _fileName = fileName;
            for (int i = 0; i < ui->playlistsTabWidget->count(); i++)
                _dataStorage->addPlaylistModel((PlaylistModel*) ( (PlaylistTableView*) ui->playlistsTabWidget->widget(i) )->model());

            _dataStorage->save(file);
            file.close();
            QMessageBox::information(this, tr("Saved"),tr("Listing saved."));
        }
    }
}


void MainWindow::on_openListingAction_triggered()
{
    if(_playlistPlayer->mediaPlayer()->isPlaying() || _playlistPlayer->mediaPlayer()->isPaused()){
        QMessageBox::critical(this, tr("Playlist is running"), tr("Playlist is running. \nPlease stop playlist before open a listing."));
    }else{
        //TODO Mettre test si modification de la programmation actuelle à la place
        verifSave();
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open listing"), "", tr("OPP file (*.opp)"));
        if (fileName.isEmpty()) {
            return;
        } else {
            QFile file(fileName);
            if (!file.open(QIODevice::ReadWrite)) {
                QMessageBox::information(this, tr("Unable to open file."),file.errorString());
            }

            _fileName = fileName;

            _dataStorage->load(file);
            updatePlaylistListCombox();
            file.close();

            ui->progEdit->setText(_dataStorage->projectTitle());
            ui->notesEdit->setText(_dataStorage->projectNotes());

        }
    }
}


void MainWindow::on_newListingAction_triggered()
{
    if(_playlistPlayer->mediaPlayer()->isPlaying() || _playlistPlayer->mediaPlayer()->isPaused()){
        QMessageBox::critical(this, tr("Playlist is running"), tr("Playlist is running. \nPlease stop playlist before new listing."));
    }else{
        //TODO Mettre test si modification de la programamtion actuelle à la place
        verifSave();
        _dataStorage->clear();
        // FIX : ref 0000001
        // add empty tab and remove all other one (init state)
        createPlaylistTab();
        int size = ui->schedulePlaylistListComboBox->count();

        while (ui->playlistsTabWidget->count() > 1)
            ui->playlistsTabWidget->removeTab(0);

        ui->progEdit->clear();
        ui->notesEdit->clear();
        for(int i = 0; i < size; i++){
            ui->schedulePlaylistListComboBox->removeItem(i);
        }
        updatePlaylistListCombox();
        updateDetails();

        _fileName = "";
    }
}

void MainWindow::on_quitAction_triggered()
{
    //TODO Mettre test si modification de la programamtion actuelle à la place
    if(_mediaListModel->rowCount()!=0)
    {
        //0:ne pas enregistrer ni quitter / 1:ne pas enregistrer mais quitter / 2:enregistrer puis quitter
        int choice = QMessageBox::warning(this, tr("Save"), tr("Do you want to save the current listing ? \nOtherwise unsaved data will be lost."), tr("Cancel"), tr("No"), tr("Yes"));
        if(choice == 1)
        {
            close();
        }
        else if(choice == 2)
        {
            on_saveAction_triggered();
            close();
        }
    }
    else
    {
        close();
    }
}

/***********************************************************************************************\
                                          Automation
\***********************************************************************************************/


void MainWindow::updatePlaylistListCombox()
{
    QStringList tabs;

    for (int i = 0; i < ui->playlistsTabWidget->count(); i++)
        tabs << ui->playlistsTabWidget->tabText(i);

    ui->schedulePlaylistListComboBox->clear();
    ui->schedulePlaylistListComboBox->addItems(tabs);
}

void MainWindow::on_scheduleDeleteButton_clicked()
{
    QItemSelectionModel *selectionModel = ui->scheduleTableView->selectionModel();

    while (selectionModel->selectedRows().count() != 0) {
        QModelIndex index = selectionModel->selectedRows().first();
        _scheduleListModel->removeSchedule(index.row());
    }
}

void MainWindow::on_scheduleAddButton_clicked()
{
    QDateTime launchAt(ui->scheduleLaunchAtDateEdit->date(), ui->scheduleLaunchAtTimeEdit->time());
    const int playlistIndex = ui->schedulePlaylistListComboBox->currentIndex();

    if (launchAt <= QDateTime::currentDateTime()) {
        QMessageBox::critical(this, tr("Schedule validation"), QString(tr("The schedule launch date must be later than the current date.")));
        return;
    }

    Playlist *playlist = playlistAt(playlistIndex);
    Schedule *schedule = new Schedule(playlist, launchAt);

    if (_scheduleListModel->isSchedulable(schedule)) {
        connect(schedule, SIGNAL(triggered(Playlist*)), _playlistPlayer, SLOT(playPlaylist(Playlist*)));
        _scheduleListModel->addSchedule(schedule);
    } else {
        QMessageBox::critical(this, tr("Schedule validation"), QString(tr("A playlist was already scheduled between the %1 and %2, \nPlease choose an other launch date."))
                              .arg(schedule->launchAt().toString())
                              .arg(schedule->finishAt().toString())
                              );
        delete schedule;
    }
}

void MainWindow::on_scheduleDelayButton_clicked()
{
    int delay = ui->scheduleDelaySpinBox->value() /*min*/;

    int err = _scheduleListModel->delayAll(delay * 60 * 1000 /*ms*/);
    if(err == 1)
        QMessageBox::critical(this, tr("Delay validation"), tr("With this delay a playlist starts before the end of the current playlist. \nPlease choose an other delay."));
    else if(err == 2)
        QMessageBox::critical(this, tr("Delay validation"), tr("With this delay a playlist starts before the current date. \nPlease choose an other delay."));
}

void MainWindow::on_scheduleToggleEnabledButton_toggled(bool checked)
{
    if (checked) {
        ui->scheduleToggleEnabledButton->setText(tr("Stop automation"));
    } else {
        ui->scheduleToggleEnabledButton->setText(tr("Start automation"));
    }
}


/***********************************************************************************************\
                                          Helpers
\***********************************************************************************************/


Playback* MainWindow::selectedPlayback() const {
    QModelIndexList indexes = currentPlaylistTableView()->selectionModel()->selectedRows();

    if (indexes.count() == 0)
        return NULL;

    return currentPlaylistModel()->playlist()->at(indexes.first().row());
}

PlaylistTableView* MainWindow::currentPlaylistTableView() const
{
    return (PlaylistTableView*) ui->playlistsTabWidget->currentWidget();
}

PlaylistModel* MainWindow::currentPlaylistModel() const
{
    return (PlaylistModel*) currentPlaylistTableView()->model();
}

Playlist* MainWindow::playlistAt(int index) const
{
    return ( (PlaylistModel*) ( (PlaylistTableView*) ui->playlistsTabWidget->widget(index) )->model() )->playlist();
}

void MainWindow::on_aboutAction_triggered()
{
    _aboutdialog->show();
    _aboutdialog->raise();
    _aboutdialog->activateWindow();

}

void MainWindow::on_updateAction_triggered(){
    Updater up;
    up.checkVersion();
}

QList<QWidget*> MainWindow::getLockedWidget()
{
    QList<QWidget*> lockedWidget;

    lockedWidget << ui->seekWidget;
    lockedWidget << ui->disableButton;

    lockedWidget << ui->audioTrackComboBox;
    lockedWidget << ui->videoTrackComboBox;
    lockedWidget << ui->subtitlesTrackComboBox;
    lockedWidget << ui->subtitlesSyncSpinBox;
    lockedWidget << ui->ratioComboBox;

    lockedWidget << ui->gammaSpinBox;
    lockedWidget << ui->contrastSpinBox;
    lockedWidget << ui->brightnessSpinBox;
    lockedWidget << ui->saturationSpinBox;
    lockedWidget << ui->hueSpinBox;

    lockedWidget << ui->audioSyncDoubleSpinBox;
    lockedWidget << ui->audioGainDoubleSpinBox;

    lockedWidget << ui->advancedSettingsButton;
    lockedWidget << ui->advancedPictureSettingsButton;

    lockedWidget << ui->playlistsTabWidget;

    lockedWidget << ui->addPlaylistButton;
    lockedWidget << ui->editNamePlaylistButton;
    lockedWidget << ui->playlistUpButton;
    lockedWidget << ui->deletePlaylistItemButton;
    lockedWidget << ui->playlistDownButton;

    lockedWidget << ui->playerControlsWidget;

    return lockedWidget;
}

void MainWindow::setScreenshot(QString url)
{
    QPixmap pixmap(url);
    ui->screenBack->setPixmap(pixmap.scaled(ui->screenBack->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
}


/****** CHARGEMENT DES PLUGINS ***********/

void MainWindow::loadPlugins(){

    QDir pluginsDir = QDir(qApp->applicationDirPath());
    pluginsDir.cd("pluginsOPP");
    if(pluginsDir.exists()){
        foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
            QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
            QObject *plugin = loader.instance();
            if (plugin)
            {

                OCPM * op = qobject_cast<OCPM *>(plugin);
                if (op != NULL)
                {
                    _ocpmPlugin = op;
                    op->setFilename(_selectedMediaName);
                    ui->menuPlugins->addAction(op->getName(),op,SLOT(launch()));
                    QPushButton *button = new QPushButton(tr("OCPM change screenshot"));
                    button->setIcon(QIcon(":icons/resources/glyphicons/glyphicons_008_film.png"));
                    ui->horizontalLayout_11->addWidget(button);

                    connect ( button, SIGNAL( clicked() ), this, SLOT( ocpmSecondaryAction() ) );
                }
            }
        }
    }

}

void MainWindow::ocpmSecondaryAction(){
    if(currentPlaylistTableView() != NULL &&  currentPlaylistTableView()->selectionModel() != NULL){
        QModelIndexList indexes = currentPlaylistTableView()->selectionModel()->selectedRows();
        if(indexes.count() > 0){
            _ocpmPlugin->secondaryAction();
            setSelectedMediaTimeByIndex(indexes.first().row());
        }
    }
}

/****** Chargement des SCREENSHOTS ******/
void MainWindow::setSelectedMediaNameByIndex(int idx){
    if(idx == -1){
        *_selectedMediaName = "";
    }else{
        Media *m = currentPlaylistModel()->playlist()->at(idx)->media();
        *_selectedMediaName = m->getLocation();
    }
}

void MainWindow::setSelectedMediaTimeByIndex(int idx)
{
    if(idx == -1)
    {
        ui->labelBefore->setText("");
        ui->label_screen->setText("");
        ui->label_stream->setText("");
        ui->label_none->setText("");
        ui->labelAfter->setText("");
        ui->screenAfter->clear();
        ui->screenBefore->clear();
        ui->screenBack->clear();
        ui->screen_none->clear();

    }
    else
    {
        QString display = "hh:mm:ss";

        Media *m = currentPlaylistModel()->playlist()->at(idx)->media();
        QTime time = QTime(0,0,0,0).addMSecs(m->duration());
        ui->label_screen->setText(time.toString(display));
        ui->label_stream->setText(time.toString(display));
        ui->label_none->setText(time.toString(display));

        QPixmap pixmap;
        if(m->isAudio())
        {
            pixmap.load( QString(":/icons/resources/images/intertitleSound.jpg").replace("/",QDir::separator()).toStdString().c_str() );

        }
        else if(m->isImage())
        {
            pixmap.load(m->getLocation().toStdString().c_str());
        }
        else
        {
            //pixmap.load(("./screenshot/"+m->getLocation().replace(QString(QDir::separator()),QString("_"))+".png").toStdString().c_str());
            QString path = "./screenshot/";
            path = path.replace("/",QDir::separator());
            path +=  m->getLocation().replace(QDir::separator(),"_").remove(":");
            path += ".png";
            pixmap.load((path.toStdString().c_str()));

        }
        ui->screen_none->setPixmap(pixmap.scaled(ui->screen_none->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
        ui->screenBack->setPixmap(pixmap.scaled(ui->screenBack->size(), Qt::KeepAspectRatio, Qt::FastTransformation));

        //gérer retour son
        if(idx > 0)
        {
            Media *mB = currentPlaylistModel()->playlist()->at(idx-1)->media();
            QTime timeB = QTime(0,0,0,0).addMSecs(mB->duration());
            QPixmap pixmapB;
            if(mB->isAudio())
            {
                pixmapB.load( QString(":/icons/resources/images/intertitleSound.jpg").replace("/",QDir::separator()).toStdString().c_str() );

            }
            else if(mB->isImage())
            {
                pixmapB.load(mB->getLocation().toStdString().c_str());
            }
            else
            {
                QString path = "./screenshot/";
                path = path.replace("/",QDir::separator());
                path +=  mB->getLocation().replace(QDir::separator(),"_").remove(":");
                path += ".png";
                pixmapB.load((path.toStdString().c_str()));
            }
            ui->screenBefore->setPixmap(pixmapB.scaled(ui->screenBefore->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
            ui->labelBefore->setText(timeB.toString(display));
        }
        else
        {
            //QPixmap pixmapB(QString(":/icons/resources/glyphicons/glyphicons_207_remove_2.png").replace("/",QDir::separator()).toStdString().c_str() );
            //ui->screenBefore->setPixmap(pixmapB.scaled(ui->screenBefore->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
            //ui->labelBefore->setText("00:00:00");
            ui->screenBefore->clear();
            ui->labelBefore->setText("");

        }

        if(currentPlaylistModel()->playlist()->count()-1 - idx > 0)
        {
            Media *mA = currentPlaylistModel()->playlist()->at(idx+1)->media();
            QTime timeA = QTime(0,0,0,0).addMSecs(mA->duration());

            QPixmap pixmapA;
            if(mA->isAudio())
            {
                pixmapA.load( QString(":/icons/resources/images/intertitleSound.jpg").replace("/",QDir::separator()).toStdString().c_str() );

            }
            else if(mA->isImage())
            {
                pixmapA.load(mA->getLocation().toStdString().c_str());
            }
            else
            {
                QString path = "./screenshot/";
                path = path.replace("/",QDir::separator());
                path +=  mA->getLocation().replace(QDir::separator(),"_").remove(":");
                path += ".png";
                pixmapA.load((path.toStdString().c_str()));
            }
            ui->screenAfter->setPixmap(pixmapA.scaled(ui->screenAfter->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
            ui->labelAfter->setText(timeA.toString(display));
        }
        else
        {
            //QPixmap pixmapA(QString(":/icons/resources/glyphicons/glyphicons_207_remove_2.png").replace("/",QDir::separator()).toStdString().c_str());
            //ui->screenAfter->setPixmap(pixmapA.scaled(ui->screenAfter->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
            //ui->labelAfter->setText("00:00:00");
            ui->screenAfter->clear();
            ui->labelAfter->setText("");

        }
    }
}

void MainWindow::updateBackTime(const int &time)
{
    QString display = "hh:mm:ss";
    Media *m = _playlistPlayer->mediaPlayer()->media();
    int t = m->duration()-time;
    QTime currentTime = QTime(0,0,0,0).addMSecs(t);

    ui->label_screen->setText(currentTime.toString(display));
    ui->label_stream->setText(currentTime.toString(display));
    ui->label_none->setText(currentTime.toString(display));
}

void MainWindow::setScreensBack(QString urlA)
{
    QPixmap pixmapA(urlA);
    ui->screen_none->setPixmap(pixmapA.scaled(ui->screenAfter->size(), Qt::KeepAspectRatio, Qt::FastTransformation));

}

void MainWindow::on_subtitlesEncodecomboBox_currentIndexChanged(int index)
{
    if (index == -1 || index == 0)
        return;

    Playback *playback = selectedPlayback();

    if (playback) {

        playback->mediaSettings()->setSubtitlesEncode(index);
    }

    currentPlaylistModel()->updateLayout();
}

void MainWindow::updateCurrentScreenshot(){
    if(currentPlaylistTableView() != NULL &&  currentPlaylistTableView()->selectionModel() != NULL){
        QModelIndexList indexes = currentPlaylistTableView()->selectionModel()->selectedRows();
        if(indexes.count() > 0){
            setSelectedMediaTimeByIndex(indexes.first().row());
        }
    }
}


QString MainWindow::scheduleToHml(){
    QString out;
    out = "<!DOCTYPE html>\n"
           "<html>\n"
               "<head>\n"
                   "<title>OPP Schedule</title>\n"
                   "<meta charset='UTF-8'>\n"
                   "<meta name='viewport' content='width=device-width'>\n"
               "</head>\n"
               "<body>\n"
                   "<div style='text-align:center; margin:0; padding:0;'>\n"
                        "<h3>Schedule OPP</h3>"
                        "<table border='1' cellspacing='0' cellpadding='5' style:'margin: auto;'>\n"
                           "<thead>\n"
                               "<tr>\n"
                                   "<th>Launch at</th>\n"
                                   "<th>Finish at</th>\n"
                                   "<th>Playlist</th>\n"
                                   "<th>State</th>\n"
                               "</tr>\n"
                           "</thead>\n"
                           "<tbody>\n";

        foreach(Schedule *schedule, _scheduleListModel->scheduleList()){
            out +=  "<tr style='background-color: lightgray; font-weight:bold;'>\n"
                +  QString("<td>%1</td>\n").arg(schedule->launchAt().toString())
                +  QString("<td>%1</td>\n").arg(schedule->finishAt().toString())
                +  QString("<td style='color:#ff03cc'>%1</td>\n").arg(schedule->playlist()->title())
                +  QString::fromUtf8("<td>%1</td>\n").arg(QString(schedule->isActive() ? QString("Active") : schedule->isExpired() ? QString::fromUtf8("Expirée") : QString("Annulée")))
                +  "</tr>\n";
                foreach(Playback *playback, schedule->playlist()->playbackList()){
                    out +=  "<tr>\n"
                        +  QString("<td></td>\n")
                        +  QString("<td colspan='2'>%1</td>\n").arg(playback->media()->name())
                        +  QString(QString("<td>%1</td>\n").arg( msecToQTime(playback->media()->duration()).toString("hh:mm:ss")))
                        +  "</tr>\n";
                }
        }

        out +=             "</table>\n"
                           "</div>\n"
                       "</body>\n"
                   "</html>\n";

       return out.toUtf8();
}

void MainWindow::on_viewExportPDFButton_clicked()
{
    _exportPDF->setHtml(scheduleToHml());
    _exportPDF->show();
}


void MainWindow::myMessageHandler(QtMsgType type, const char* msg)
{
   QString dt = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
   QString txt = QString("[%1]\n").arg(dt);

   switch (type)
   {
      case QtDebugMsg:
         txt += QString("\t{Debug} %1").arg(msg);
         break;
      case QtWarningMsg:
         txt += QString("\t{Warning} %1").arg(msg);
         break;
      case QtCriticalMsg:
         txt += QString("\t{Critical} %1").arg(msg);
         break;
      case QtFatalMsg:
         txt += QString("\t{Fatal} %1").arg(msg);
         abort();
         break;
   }

   QFile outFile("opp.log");
   outFile.open(QIODevice::WriteOnly | QIODevice::Append);

   QTextStream textStream(&outFile);
   textStream << txt << endl;

   LoggerSingleton::getInstance()->writeMessage(txt);
}
