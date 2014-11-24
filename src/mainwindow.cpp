/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
 *          Baptiste Rozière <bapt.roziere@gmail.com>
 *          Hamza Haddar <ham.haddar@gmail.com>
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
#include <QSignalMapper>

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

#include "VLCApplication.h"
#include "media.h"
#include "PlaylistPlayer.h"
#include "MediaPlayer.h"
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
    _playerControlWidget(NULL),
    _selectedMediaName(NULL),
    _ocpmPlugin(NULL),
    _exportPDF(NULL),
    _vlcMire(NULL),
    _mpMire(NULL),
    _mireMire(NULL),
    _pbMire(NULL),
    _vWMire(NULL),
    _timerOut(NULL)
{
    ui->setupUi(this);

    /****************** internal core initalization *********************/
    _app = new VLCApplication();
    _playlistPlayer = new PlaylistPlayer(_app->vlcInstance(), this);

    _timerOut = new QTimer();
    _timerOut->connect(_timerOut, SIGNAL(timeout()), this, SLOT(showTimeOut()));
    _timerOut->start(1000);

    /********************** The video window ****************************/
    _videoWindow = new VideoWindow(this, VideoWindow::WINDOW);
    connect(_videoWindow, SIGNAL(closed()), _playlistPlayer, SLOT(stop()));

    /************ Initialize the media player *************/
    MediaPlayer* mediaPlayer = _playlistPlayer->mediaPlayer();
    mediaPlayer->setVideoView( (VideoView*) _videoWindow->videoWidget() );
    mediaPlayer->setVideoBackView( (VideoView*) ui->backWidget );
    mediaPlayer->initStream();

    connect(mediaPlayer, SIGNAL(stopped()), this, SLOT(stop()));

    /**
     * Create the widget which handle the playlist player.
     * It have to be created before the locker.
     */
    _playerControlWidget = new PlayerControlWidget(_playlistPlayer, this);

    /*********** Media list model and schedule list model *************/
    _mediaListModel = new MediaListModel();
    _scheduleListModel = new ScheduleListModel();

    connect(ui->scheduleToggleEnabledButton, SIGNAL(toggled(bool)), _scheduleListModel, SLOT(toggleAutomation(bool)));

    ui->binTableView->setModel(_mediaListModel);
    ui->scheduleTableView->setModel(_scheduleListModel);

    connect(_mediaListModel, SIGNAL(mediaListChanged(int)),this, SLOT(updateProjectSummary()));
    connect(_scheduleListModel, SIGNAL(scheduleListChanged()),this, SLOT(updateProjectSummary()));

    /********************** Locker In The Status Bar ***********************/
    _locker = new Locker(this);
    _statusWidget = new StatusWidget;

    ui->statusBar->addPermanentWidget(_statusWidget);
    connect(_mediaListModel, SIGNAL(mediaListChanged(int)), _statusWidget, SLOT(setMediaCount(int)));

    connect(_locker, SIGNAL(toggled(bool)), _statusWidget->lockButton(), SLOT(setChecked(bool)));
    connect(_statusWidget->lockButton(), SIGNAL(clicked(bool)), _locker, SLOT(toggle(bool)));

    _lockSettingsWindow = new LockSettingsWindow(_locker, this);

    /************** Create the playlist handler widget ****************/
    _playlistHandlerWidget = new PlaylistHandlerWidget(this);
    _playlistTabWidget = _playlistHandlerWidget->playlistTabWidget();

    /************************ Add the widgets *************************/
    ui->verticalLayout->addWidget(_playlistHandlerWidget);
    ui->verticalLayout->addWidget(_playerControlWidget);

    /**** Create a first default playlist *******/
    _playlistTabWidget->createTab();

    /***************** Give the widgets to the locker ***************/
    _locker->setLockedWidgets(getLockedWidget());

    /************ show/hide pannel actions *********************/
    connect(ui->resumeDetailsAction, SIGNAL(toggled(bool)), ui->projectTabWidget, SLOT(setVisible(bool)));
    connect(ui->binAction, SIGNAL(toggled(bool)), ui->binGroupBox, SLOT(setVisible(bool)));
    connect(ui->automationAction, SIGNAL(toggled(bool)), ui->scheduleGroupBox, SLOT(setVisible(bool)));
    connect(ui->statusBarAction, SIGNAL(toggled(bool)), ui->statusBar, SLOT(setVisible(bool)));

    /*************** Connect actions to the playlist handler widget ****************/
    connect(ui->newPlaylistAction, SIGNAL(triggered()), _playlistHandlerWidget->playlistControlWidget(), SLOT(addPlaylist()));
    connect(ui->removePlaylistItemAction, SIGNAL(triggered()), _playlistHandlerWidget->playlistControlWidget(), SLOT(removePlaylistItem()));
    connect(ui->renamePlaylistAction, SIGNAL(triggered()), _playlistHandlerWidget->playlistControlWidget(), SLOT(renamePlaylist()));

    /********** set video mode actions data *********************/
    ui->actionProjection->setData(QVariant(VideoWindow::PROJECTION));
    ui->actionWindow->setData(QVariant(VideoWindow::WINDOW));

    _dataStorage = new DataStorage(_app, this);
    _dataStorage->setMediaListModel(_mediaListModel);
    _dataStorage->setScheduleListModel(_scheduleListModel);

    connect(ui->progEdit,SIGNAL(textChanged(QString)), _dataStorage, SLOT(setProjectTitle(QString)));

    /************* Set the default schedule information ***********/
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
        //ui->stackedWidget->setCurrentIndex(1);
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
    ui->textEdit_Codecs->append("<span style=\"text-decoration: underline;\">Audio codecs:</span>");
    ui->textEdit_Codecs->append("");
    ui->textEdit_Codecs->append("<span style=\"text-decoration: underline;\">Video codecs:</span>");

    _logger = LoggerSingleton::getInstance();
    _logger->setTextEdit(ui->label_5);

    /*** load plugins ***/
    loadPlugins();

    //Redimensionnement colonnes Bin
    ui->binTableView->setColumnWidth(1,80);
    ui->binTableView->setColumnWidth(2,60);
    ui->scheduleTableView->setColumnWidth(0,130);
    ui->scheduleTableView->setColumnWidth(1,130);

    ui->menuPlay_a_test_pattern->clear();
    QDir mires("mires");
    QStringList filesList = mires.entryList(QDir::Files);
    QString fileName;

    QSignalMapper* signalMapper = new QSignalMapper (this) ;
    foreach(fileName, filesList)
    {
        QAction *action = new QAction(fileName,this);
        ui->menuPlay_a_test_pattern->addAction(action);

        connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
        signalMapper -> setMapping (action, fileName) ;
    }

    QAction *actionClose = new QAction("Close test pattern.", this);
    ui->menuPlay_a_test_pattern->addAction(actionClose);

    connect(actionClose,SIGNAL(triggered()),this,SLOT(closeWindowTestPattern()));
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(playMire(QString)));

    QString styleScreen = QString(
        "QLabel {"
        "   background: rgb(0, 0, 0);"
        "   border: 0;"
        "}"
    );

    ui->screenBefore->setStyleSheet(styleScreen);
    ui->screenAfter->setStyleSheet(styleScreen);
    ui->screen_none->setStyleSheet(styleScreen);
    ui->screenBack->setStyleSheet(styleScreen);

    ui->screenAfter->clear();
    ui->screenBefore->clear();
    ui->screenBack->clear();
    ui->screen_none->clear();

    /***************  Set the style *********************/
    setStyleSheet(QString(
        "QPushButton{"
        "   margin: 0px 0px 0px 0px;"
        "   padding: 5px 2px 5px 2px;"
        "   border-radius: 0px;"
        "   border: 1px solid rgb(190, 190, 190);"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 rgb(240,240,240), stop:1 rgb(223,223,223));"
        "}"
        "QPushButton:!enabled{"
        "   border: 1px solid rgb(210, 210, 210);"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 rgb(255,255,255), stop:1 rgb(238,238,238));"
        "}"
        "QPushButton:pressed{"
        "   border: 1px solid rgb(153, 153, 153);"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 rgb(220,220,220), stop:1 rgb(203,203,203));"
        "}"
        "QPushButton:selected{"
        "   border: 1px solid rgb(153, 153, 153);"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 rgb(220,220,220), stop:1 rgb(203,203,203));"
        "}"
        "QPushButton:focus {"
        "   border-color: rgb(120, 120, 120);" /* make the default button prominent */
        "}"
        "QTabWidget::pane{"
        "   padding: 2px 0px 0px 0px;"
        "   border-radius: 0px;"
        "   border: 1px solid rgb(180,180,180);"
        "   background: rgb(240, 240, 240);"
        "   position: absolute;"
        "   top: -1px;"
        "}"
        "QTabWidget::tab-bar {"
        "   alignment: left;"
        "}"
        "QTabBar::text{"
        "   margin: 0;"
        "}"
        "QTabBar::tab{"
        "	padding: 4px;"
        "	border-top: 1px solid rgb(180, 180, 180);"
        "	border-left: 1px solid rgb(180, 180, 180);"
        "	border-right: 1px solid rgb(180, 180, 180);"
        "	border-bottom: 1px solid rgb(240, 240, 240);"
        "}"
        "QTabBar::tab:selected {"
        "	background-color: rgb(240, 240, 240);"
        "   margin-left: -2px;" /* expand/overlap to the left and right by 4px */
        "   margin-right: -2px;"
        "}"
        "QTabBar::tab:first:selected {"
        "   margin-left: 0;" /* the first selected tab has nothing to overlap with on the left */
        "}"

        "QTabBar::tab:last:selected {"
        "   margin-right: 0;" /* the last selected tab has nothing to overlap with on the right */
        "}"
        "QTabBar::tab:only-one {"
        "   margin: 0;" /* if there is only one tab, we don't want overlapping margins */
        "}"
        "QTabBar::tab:!selected {"
        "   margin-top: 4px;" /* make non-selected tabs look smaller */
        "	background: rgb(210, 210, 210);"
        "	border-bottom: 1px solid rgb(180, 180, 180);"
        "}"
    ));

    // Start/stop automation button
    ui->scheduleToggleEnabledButton->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 rgb(255,220,220), stop:1 rgb(255,203,203));"
        "border: 1px solid rgb(200, 170, 170);"
        "font-size: 13px;"
        "margin: 1px 0px 0px 0px;"
        "padding-right: 3px;"
        "padding-left: 3px;"
        "padding-top: 3px;"
        "padding-bottom: 3px;"
    );
    ui->scheduleToggleEnabledButton->setFocusPolicy(Qt::NoFocus);

    // Create the shortcut which enable to switch video modes
    _f1_shortcut = new QShortcut(QKeySequence("f1"), this);

    connect(_f1_shortcut, SIGNAL(activated()), this, SLOT(switchVideoMode()));
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
    if(_timerOut != NULL)
        delete _timerOut;
    LoggerSingleton::destroyInstance();

    /** record the last opened listing to open it automatically the next time */
    QSettings settings("opp", "opp");
    settings.setValue("lastOpenedListing", _fileName);
}

void MainWindow::openListing(QString fileName) {
    if (fileName.isEmpty()) {
        return;
    } else {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadWrite)) {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        }

        _fileName = fileName;

        _dataStorage->load(file);
        updatePlaylistListCombox();
        file.close();

        ui->progEdit->setText(_dataStorage->projectTitle());
        ui->notesEdit->setText(_dataStorage->projectNotes());
    }
}

QLabel* MainWindow::screenBefore() const
{
    return ui->screenBefore;
}

/***********************************************************************************************\
                                          Project Details/Summary
\***********************************************************************************************/

void MainWindow::updateProjectSummary()
{
    ui->countMoviesLabel->setText( QString::number(_mediaListModel->countMovies()) );
    ui->countPicturesLabel->setText( QString::number(_mediaListModel->countPictures()) );
    ui->totalDurationLabel->setText( _scheduleListModel->totalDuration().toString("hh:mm:ss") );
}

void MainWindow::updateDetails() {
    ui->textEdit_Codecs->clear();
    ui->textEdit_Codecs->append("<span style=\"text-decoration: underline;\">Audio codecs:</span>");
    for(int i=0; i< _playlistTabWidget->count();i++) {
        PlaylistModel *model = (PlaylistModel*) ((PlaylistTableView*) _playlistTabWidget->widget(i))->model();
        foreach(Playback *playback, model->playlist()->playbackList()) {
            foreach(AudioTrack audioTrack, playback->media()->audioTracks()) {
                QString codec = audioTrack.codecDescription();
                if(!ui->textEdit_Codecs->toHtml().contains(codec))
                    ui->textEdit_Codecs->append(codec);
            }
        }
    }
    ui->textEdit_Codecs->append("");
    ui->textEdit_Codecs->append("<span style=\"text-decoration: underline;\">Video codecs:</span>");
    for(int i=0; i< _playlistTabWidget->count();i++) {
        PlaylistModel *model = (PlaylistModel*) ((PlaylistTableView*) _playlistTabWidget->widget(i))->model();
        foreach(Playback *playback, model->playlist()->playbackList()) {
            foreach(VideoTrack videoTrack, playback->media()->videoTracks()) {
                QString codec = videoTrack.codecDescription();
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

    // Import medias
    foreach (QString fileName, fileNames) {
        addMedia(fileName);
    }

    takeScreenshot(fileNames);
}

int MainWindow::addMedia(QString location)
{
    Media *media = new Media(location, _app->vlcInstance());

    if (media->exists() == false) {
        QMessageBox::warning(this, tr("Import media"), QString(tr("The file %1 does not exist. Maybe it was deleted.")).arg(media->location()));
        delete media;
        return -1;
    }

    int index = _mediaListModel->index(media);
    if (index == -1){
        index = _mediaListModel->mediaList().count();
        _mediaListModel->addMedia(media);
        return index;
    }else{
        delete media;
        return index;
    }
}

void MainWindow::takeScreenshot(QString fileName)
{
    QStringList list;
    list.append(fileName);

    takeScreenshot(list);
}

void MainWindow::takeScreenshot(QStringList fileNames)
{
    VideoWindow win(this);
    VideoWidget *vw = win.videoWidget();
    libvlc_instance_t *vlc = libvlc_new(0, NULL);
    libvlc_media_player_t *vlcMP = libvlc_media_player_new(vlc);

    WId wid = vw->request();

    if (wid) {
        #if defined(Q_OS_WIN)
                libvlc_media_player_set_hwnd(vlcMP, (void *)wid);
        #elif defined(Q_OS_MAC)
                libvlc_media_player_set_nsobject(vlcMP, (void *)wid);
        #elif defined(Q_OS_UNIX)
                libvlc_media_player_set_xwindow(vlcMP, wid);
        #endif
    }
    QDir appPath = QDir(qApp->applicationDirPath());

    if(!QDir(appPath.path() + "/screenshot").exists())
    {
        QDir().mkdir(appPath.path() + "/screenshot");
    }

    foreach (QString fileName, fileNames) {
        Media *media = new Media(fileName, _app->vlcInstance());

        QString screenPath = appPath.path() + "/screenshot/";
        screenPath = screenPath.replace("/",QDir::separator());
        screenPath +=  media->getLocation().replace(QDir::separator(),"_").remove(":");
        screenPath += ".png";

        if(!QFile(screenPath).exists() && !media->isAudio() && !media->isImage())
        {
            libvlc_media_player_set_media(vlcMP, media->core());
            libvlc_media_add_option(media->core(),":noaudio");
            libvlc_media_player_play(vlcMP);
            libvlc_media_player_set_position(vlcMP, 0.5f);
            unsigned* width = new unsigned(0);
            unsigned* height = new unsigned(0);
            libvlc_video_get_size(vlcMP, 0, width, height);

            // reduce the size to a suitable size
            unsigned denom;
            unsigned denomH = *height / (unsigned)ui->screen_none->height();
            unsigned denomW = *width / (unsigned)ui->screen_none->width();
            if(denomH < denomW)
                denom = denomH;
            else
                denom = denomW;

            if(denom > 1){
                *width = *width/denom;
                *height = *height/denom;
            }

            wait(60);
            libvlc_video_take_snapshot(vlcMP, 0, screenPath.toStdString().c_str(), *width, *height);

            /*** Wait the screenshot creation to release the function ****/
            while(!QFile::exists(screenPath)){
                wait(3);
            }

            delete width;
            delete height;
        }
        libvlc_media_player_stop(vlcMP);
    }

    libvlc_media_player_release(vlcMP);
    libvlc_vlm_release(vlc);
}

void MainWindow::on_binDeleteMediaButton_clicked()
{
    QItemSelectionModel *selectionModel = ui->binTableView->selectionModel();
    int nbMedia = selectionModel->selectedRows().length();
    for(int i=0;i<nbMedia;i++){
        bool toDel = true;

        QModelIndex index = selectionModel->selectedRows().at(i);
        Media *media = _mediaListModel->mediaList()[index.row()];

        if (media->isUsed()) {
            //If user answer Yes
            if (1 == QMessageBox::warning(this, media->name(), tr("This media is used. All references of this media into playlists will be deleted too.\n Are you sure to remove this media ?") ,tr("No"), tr("Yes")))
            {
                int countPlaylists = _playlistTabWidget->count();
                for (int i = 0; i < countPlaylists; i++) {
                    PlaylistModel *model = (PlaylistModel*) ((PlaylistTableView*) _playlistTabWidget->widget(i))->model();
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
        if(_playlistPlayer->mediaPlayer()->isPlaying())
        {
            _playlistPlayer->mediaPlayer()->playStream();
        }
    }
    else
    {
        ui->disableButton->setText(tr("Enable"));
        _playlistPlayer->mediaPlayer()->setBackMode(MediaPlayer::NONE);
        if(_playlistPlayer->mediaPlayer()->isPlaying())
        {
            _playlistPlayer->mediaPlayer()->stopStream();
        }
    }
}


void MainWindow::openAdvancedSettingsWindow()
{
    _advancedSettingsWindow->setPlayback(selectedPlayback());
    _advancedSettingsWindow->show();
    _advancedSettingsWindow->raise();
    _advancedSettingsWindow->activateWindow();
}

void MainWindow::openAdvancedPictureSettings()
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
        ui->generalSettingsWidget->setEnabled(false);
        ui->audioSettingsWidget->setEnabled(false);
        ui->pictureSettingsWidget->setEnabled(false);
        _playerControlWidget->setEnabled(false);
        return;
    }

    ui->audioTrackComboBox->blockSignals(true);
    ui->videoTrackComboBox->blockSignals(true);
    ui->subtitlesTrackComboBox->blockSignals(true);

    if(!_locker->isLock())
    {
        ui->generalSettingsWidget->setEnabled(true);
        ui->audioSettingsWidget->setEnabled(true);
        ui->pictureSettingsWidget->setEnabled(true);
        _playerControlWidget->setEnabled(true);
    }
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
    ui->audioSyncDoubleSpinBox->setValue(playback->mediaSettings()->audioSync());

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

    ui->subtitlesEncodecomboBox->setCurrentIndex( playback->mediaSettings()->subtitlesEncode() );

    ui->audioTrackComboBox->blockSignals(false);
    ui->videoTrackComboBox->blockSignals(false);
    ui->subtitlesTrackComboBox->blockSignals(false);

    ui->scheduleLaunchAtTimeEdit->setCurrentSectionIndex(1);
}


/***********************************************************************************************\
                                          Player
\***********************************************************************************************/

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

void MainWindow::switchVideoMode(){
    if(!_locker->isLock()){
        foreach(QAction *action, ui->menuVideoMode->actions()) {
            if (action->data().toInt() != (int)_projectionMode) {
                on_menuVideoMode_triggered(action);
                return;
            }
        }
    }
}

void MainWindow::stop(){
    ui->label_timeRemaining->setText("00:00:00");
}

/***********************************************************************************************\
                                          Project import/export
\***********************************************************************************************/

int MainWindow::verifSave ()
{
    int choice = 1;
    if(_mediaListModel->rowCount() != 0)
    {
        choice = QMessageBox::warning(this, tr("Save"), tr("Do you want to save the current listing ? \nOtherwise unsaved data will be lost."), tr("Cancel"), tr("No"), tr("Yes"));
        if(choice == 2)
        {
            on_saveAction_triggered();
        }
    }
    return choice;
}

void MainWindow::on_addMediaAction_triggered()
{
    on_binAddMediaButton_clicked();
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
            for (int i = 0; i < _playlistTabWidget->count(); i++)
                _dataStorage->addPlaylistModel((PlaylistModel*) ( (PlaylistTableView*) _playlistTabWidget->widget(i) )->model());

            _dataStorage->save(file);

            file.close();
            QMessageBox::information(this, tr("Saved"),tr("Listing saved."));
        }
    }
}

void MainWindow::on_saveAsAction_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Listing"), QDir::homePath(), tr("OPP file (*.opp)"));

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
            for (int i = 0; i < _playlistTabWidget->count(); i++)
                _dataStorage->addPlaylistModel((PlaylistModel*) ( (PlaylistTableView*) _playlistTabWidget->widget(i) )->model());

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
        if(verifSave() != 0){
            QString listingPath = QFileDialog::getOpenFileName(this, tr("Open listing"), QDir::homePath(), tr("OPP file (*.opp)"));
            openListing(listingPath);
        }
    }
}

void MainWindow::on_newListingAction_triggered()
{
    if(_playlistPlayer->mediaPlayer()->isPlaying() || _playlistPlayer->mediaPlayer()->isPaused()){
        QMessageBox::critical(this, tr("Playlist is running"), tr("Playlist is running. \nPlease stop playlist before new listing."));
    }else{
        //TODO Mettre test si modification de la programamtion actuelle à la place
        if(verifSave() != 0){
            _dataStorage->clear();

            // add empty tab and remove all other one (init state)
            _playlistTabWidget->createTab();
            int size = ui->schedulePlaylistListComboBox->count();

            while (_playlistTabWidget->count() > 1)
                _playlistTabWidget->removeTab(0);

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

    for (int i = 0; i < _playlistTabWidget->count(); i++)
        tabs << _playlistTabWidget->tabText(i);

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

    Playlist *playlist = _playlistHandlerWidget->playlistAt(playlistIndex);
    Schedule *schedule = new Schedule(playlist, launchAt);

    if (_scheduleListModel->isSchedulable(schedule)) {
        connect(schedule, SIGNAL(triggered(Playlist*)), _playlistPlayer, SLOT(playPlaylist(Playlist*)));
        connect(schedule, SIGNAL(triggered(Playlist*)), this, SLOT(needVideoWindow(Playlist*)));
        _scheduleListModel->addSchedule(schedule);
    } else {
        delete schedule;
    }
}

void MainWindow::needVideoWindow(){
    if(!_videoWindow->isVisible()){
        _videoWindow->show();
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
        ui->scheduleToggleEnabledButton->setText(tr("Stop"));
    } else {
        ui->scheduleToggleEnabledButton->setText(tr("Start"));
    }
}

void MainWindow::showTimeOut()
{
    if(!_scheduleListModel->getNextSchedule() == NULL && ui->scheduleToggleEnabledButton->isChecked())
    {

        int ecart = QDateTime::currentDateTime().secsTo(*_scheduleListModel->getNextSchedule());

        if(ecart > (60*5)){
            ui->label_timeout->setStyleSheet("QLabel { color : black; font-weight : 200;}");
        }else if (ecart > 30){
            ui->label_timeout->setStyleSheet("QLabel { color : red;font-weight : 200; }");
        }else{
            if(ecart%2 == 0)     //Even seconds -> orange label
            {
                ui->label_timeout->setStyleSheet(QString("QLabel { ")+
                                                 QString("color : white;")+
                                                 QString("font-weight : 500;")+
                                                 QString("background-color: red;")+
                                                 QString("border-style: outset;")+
                                                 QString("border-width: 2px;")+
                                                 QString("border-radius: 10px;")+
                                                 QString("border-color: black; }"));
            }
            else            //Odd seconds -> red label
            {
                ui->label_timeout->setStyleSheet(QString("QLabel { ")+
                                                 QString("color : red;")+
                                                 QString("font-weight : 500;")+
                                                 QString("background-color: white;")+
                                                 QString("border-style: outset;")+
                                                 QString("border-width: 2px;")+
                                                 QString("border-radius: 10px;")+
                                                 QString("border-color: black; }"));
            }
        }

        QTime t(0,0);
        if(ecart<86400)
        {
            t = t.addSecs(ecart);
            ui->label_timeout->setText(t.toString("hh:mm:ss"));
        }
        else
        {
            int nbDays = ecart / 86400;
            ecart -= nbDays*86400;
            t = t.addSecs(ecart);
            ui->label_timeout->setText(QString(tr("%1day(s) and ").arg(nbDays))+t.toString("hh:mm:ss"));
        }


    }
    else
    {
        QString none = tr("None Scheduled");
        if(ui->label_timeout->text() != none){
            ui->label_timeout->setStyleSheet("QLabel { color : black; font-weight : 200; }");
            ui->label_timeout->setText(none);
        }
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
    return _playlistHandlerWidget->currentPlaylistTableView();
}

PlaylistModel* MainWindow::currentPlaylistModel() const
{
    return _playlistHandlerWidget->currentPlaylistModel();
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

    lockedWidget << ui->generalSettingsWidget;
    lockedWidget << ui->audioSettingsWidget;
    lockedWidget << ui->pictureSettingsWidget;
    lockedWidget << ui->disableButton;
    lockedWidget << _playlistHandlerWidget;
    lockedWidget << _playerControlWidget;

    return lockedWidget;
}

void MainWindow::setScreenshot(QString url)
{
    QPixmap pixmap(url);
    ui->screenBack->setPixmap(pixmap.scaled(ui->screenBack->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
}

QLabel* MainWindow::screenBack() const
{
    return ui->screenBack;
}

/****** PLUGIN LOADER***********/

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
                if (op != 0)
                {
                    _ocpmPlugin = op;
                    op->setFilename(_selectedMediaName);
                    ui->menuPlugins->addAction(op->getName(),op,SLOT(launch()));
                    QPushButton *button = new QPushButton(tr("OCPM change screenshot"));
                    button->setIcon(QIcon(":icons/resources/glyphicons/glyphicons_008_film.png"));
                    ui->horizontalLayout_11->addWidget(button);

                    connect ( button, SIGNAL( clicked() ), this, SLOT( ocpmSecondaryAction() ) );
                }else{
                    OPP_GENERIC_PLUGIN *oppGEN = qobject_cast<OPP_GENERIC_PLUGIN *>(plugin);
                    if(oppGEN != 0){
                        oppGEN->setMainWindow(this);
                    }
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
    QDir appPath = QDir(qApp->applicationDirPath());
    if(idx == -1)
    {
        ui->titleBefore->setText("");
        ui->labelBefore->setText("");
        ui->title_screen->setText("");
        ui->label_screen->setText("");
        ui->title_stream->setText("");
        ui->label_stream->setText("");
        ui->title_none->setText("");
        ui->label_none->setText("");
        ui->titleAfter->setText("");
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

        // Title
        QFontMetrics metrics(ui->title_none->font());
        QString elidedText = metrics.elidedText(m->name(), Qt::ElideRight, ui->title_none->width());
        ui->title_none->setText(elidedText);
        ui->title_stream->setText(elidedText);
        ui->title_screen->setText(elidedText);

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
            QString path = appPath.path()  + "/screenshot/";
            path = path.replace("/",QDir::separator());
            path +=  m->getLocation().replace(QDir::separator(),"_").remove(":");
            path += ".png";

            /*** If we can't load the screenshot, we retake it ***/
            if(!pixmap.load((path.toStdString().c_str()))){
                takeScreenshot(m->getLocation());

                /*** Retry ****/
                pixmap.load((path.toStdString().c_str()));
            }
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
                QString path = appPath.path() + "/screenshot/";
                path = path.replace("/",QDir::separator());
                path +=  mB->getLocation().replace(QDir::separator(),"_").remove(":");
                path += ".png";

                /*** If we can't load the screenshot, we retake it ***/
                if(!pixmapB.load((path.toStdString().c_str()))){
                    takeScreenshot(mB->getLocation());

                    /*** Retry ****/
                    pixmapB.load((path.toStdString().c_str()));
                }
            }
            ui->screenBefore->setPixmap(pixmapB.scaled(ui->screenBefore->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
            ui->labelBefore->setText(timeB.toString(display));

            // Title
            QFontMetrics metrics(ui->titleBefore->font());
            QString elidedText = metrics.elidedText(mB->name(), Qt::ElideRight, ui->titleBefore->width());
            ui->titleBefore->setText(elidedText);
        }
        else
        {
            ui->screenBefore->clear();
            ui->labelBefore->setText("");
            ui->titleBefore->setText("");
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
                QString path = appPath.path() + "/screenshot/";
                path = path.replace("/",QDir::separator());
                path +=  mA->getLocation().replace(QDir::separator(),"_").remove(":");
                path += ".png";

                /*** If we can't load the screenshot, we retake it ***/
                if(!pixmapA.load((path.toStdString().c_str()))){
                    takeScreenshot(mA->getLocation());

                    /*** Retry ****/
                    pixmapA.load((path.toStdString().c_str()));
                }
            }
            ui->screenAfter->setPixmap(pixmapA.scaled(ui->screenAfter->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
            ui->labelAfter->setText(timeA.toString(display));

            // Title
            QFontMetrics metrics(ui->titleAfter->font());
            QString elidedText = metrics.elidedText(mA->name(), Qt::ElideRight, ui->titleAfter->width());
            ui->titleAfter->setText(elidedText);
        }
        else
        {
            ui->screenAfter->clear();
            ui->labelAfter->setText("");
            ui->titleAfter->setText("");
        }
    }
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
    out = "<!DOCTYPE html>"
            "<html>"
            "<head>"
            "<title>OPP Schedule</title>"
            "<meta charset=\"UTF-8\">"
            "<meta name=\"viewport\" content=\"width=device-width\">"
            "<style type=\"text/css\">"
            "body{"
            " font-family: arial;"
            " }"
            " .marge{"
            " padding-left: 10px;"
            "  padding-right: 10px; "
            " }"
            "  .page{"
            " width: 29.7cm;"
            "  //min-height: 21cm;"

            "   }    "
            "  .title {"
            " border: solid 1px #000;"
            " padding-left: 2px;"
            "text-align: center;"
            "  font-size: 16px;"
            " margin-bottom: 10px;"
            "   }"
            "   thead{"
            "   background-color: lightgray;"
            " line-height: 24px;"
            " }"
            " .droit{"
            "   text-align: right;"
            " vertical-align: top;"
            "  padding-right: 2px;"
            " }"
            " .titre{"
            "  width: 50%;"
            " }"
            "  td, img{"
            "      text-align: center;"
            "    padding: 2px;"
            "   width: 220px;"
            " height: 120px;"
            "  }"
            " </style>"
            " </head>"
            " <body>"
             "  <div class=\"page marge\">";

    foreach(Schedule *schedule, _scheduleListModel->scheduleList()){
        out+=

            " <div class=\"title\">"

              + QString("<strong>Nom de la séance : </strong>%1 - <strong>Durée total : </strong>%2 –  <strong>Début : </strong>%3 – <strong>Fin : </strong>%4 –  <strong>Jour : </strong>%5").arg(
                  schedule->playlist()->title(),
                  msecToQTime(schedule->playlist()->totalDuration()).toString("hh:mm:ss"),
                  schedule->launchAt().time().toString("hh:mm:ss"),
                  schedule->finishAt().time().toString("hh:mm:ss"),
                  schedule->launchAt().date().toString("dd/MM/yyyy")
                  )
              +

            " </div>"
        "<table border='1' cellspacing='0' class='page'>"
            "<thead>"
            "  <tr>"
            "<th>Aperçu</th>"
            "<th>Titre</th>"
            " <th>Durée</th>"
            "<th>Début</th>"
            " <th>Fin</th>"
            "    </tr>"
            "  </thead>"
            " <tbody>";
            int timeMedia = qTimeToMsec(schedule->launchAt().time());
            foreach(Playback *playback, schedule->playlist()->playbackList()){
                int dure = (int)playback->media()->duration();
                timeMedia += dure;

                QString screenPath = qApp->applicationDirPath() + "/screenshot/";
                screenPath = screenPath.replace("/",QDir::separator());
                screenPath +=  playback->media()->getLocation().replace(QDir::separator(),"_").remove(":");
                screenPath += ".png";

                if(playback->media()->isAudio())
                    screenPath = "";
                if(playback->media()->isImage())
                    screenPath = "file:///" +playback->media()->getLocation();

                out += "<tr>"
                + QString("<td class=\"img\"><img src=\"%1\" alt=\"%1\" /></td>").arg(
                    screenPath
                 )
                + QString("<td class=\"droit titre\">%1</td>").arg(
                    playback->media()->name()
                 )
                + QString("<td class=\"droit\">%1</td>").arg(
                    msecToQTime(dure).toString("hh:mm:ss")
                 )
                + QString("<td class=\"droit\">%1</td>").arg(
                    msecToQTime(timeMedia - dure).toString("hh:mm:ss")
                 )
                +QString("<td class=\"droit\">%1</td>").arg(
                     msecToQTime(timeMedia).toString("hh:mm:ss")
                 )
                +
                "  </tr>";
            }
            out+=
                  "  </tbody>"
                  " </table><br />";
    }

    out+=
          "   </div>"
          "     </body>"
          "   </html>";
    return out.toUtf8();
}

QString MainWindow::scheduleToHmlForPDF(){
    QString out;
    out = "<!DOCTYPE html>"
            "<html>"
            "<head>"
            "<title>OPP Schedule</title>"
            "<meta charset=\"UTF-8\">"
            "<meta name=\"viewport\" content=\"width=device-width\">"
            "<style type=\"text/css\">"
            "body{"
            " font-family: arial;"
            " }"
            " .marge{"
            " padding-left: 10px;"
            "  padding-right: 10px; "
            " }"
            "  .page{"
            " width: 29.7cm;"
            "  //min-height: 21cm;"

            "   }    "
            "  .title {"
            " border: solid 1px #000;"
            " padding-left: 2px;"
            "text-align: center;"
            "  font-size: 16px;"
            " margin-bottom: 10px;"
            "   }"
            "   thead{"
            "   background-color: lightgray;"
            " line-height: 24px;"
            " }"
            " .droit{"
            "   text-align: right;"
            " vertical-align: top;"
            "  padding-right: 2px;"
            " }"
            " .titre{"
            "  width: 50%;"
            " }"
            "  td, img{"
            "      text-align: center;"
            "    padding: 2px;"
            "   width: 220px;"
            " height: 120px;"
            "  }"
            " </style>"
            " </head>"
            " <body>"
             "  <div class=\"page marge\">";

    foreach(Schedule *schedule, _scheduleListModel->scheduleList()){
        out+=

            " <div class=\"title\">"

              + QString("<strong>Nom de la séance : </strong>%1 - <strong>Durée total : </strong>%2 –  <strong>Début : </strong>%3 – <strong>Fin : </strong>%4 –  <strong>Jour : </strong>%5").arg(
                  schedule->playlist()->title(),
                  msecToQTime(schedule->playlist()->totalDuration()).toString("hh:mm:ss"),
                  schedule->launchAt().time().toString("hh:mm:ss"),
                  schedule->finishAt().time().toString("hh:mm:ss"),
                  schedule->launchAt().date().toString("dd/MM/yyyy")
                  )
              +

            " </div>"
        "<table border='1' cellspacing='0' class='page'>"
            "<thead>"
            "  <tr>"
            "<th>Titre</th>"
            " <th>Durée</th>"
            "<th>Début</th>"
            " <th>Fin</th>"
            "    </tr>"
            "  </thead>"
            " <tbody>";
            int timeMedia = qTimeToMsec(schedule->launchAt().time());
            foreach(Playback *playback, schedule->playlist()->playbackList()){
                int dure = (int)playback->media()->duration();
                timeMedia += dure;

                QString screenPath = qApp->applicationDirPath() + "/screenshot/";
                screenPath = screenPath.replace("/",QDir::separator());
                screenPath +=  playback->media()->getLocation().replace(QDir::separator(),"_").remove(":");
                screenPath += ".png";

                out += "<tr>"
                + QString("<td class=\"droit titre\">%1</td>").arg(
                    playback->media()->name()
                 )
                + QString("<td class=\"droit\">%1</td>").arg(
                    msecToQTime(dure).toString("hh:mm:ss")
                 )
                + QString("<td class=\"droit\">%1</td>").arg(
                    msecToQTime(timeMedia - dure).toString("hh:mm:ss")
                 )
                +QString("<td class=\"droit\">%1</td>").arg(
                     msecToQTime(timeMedia).toString("hh:mm:ss")
                 )
                +
                "  </tr>";
            }
            out+=
                  "  </tbody>"
                  " </table><br />";
    }

    out+=
          "   </div>"
          "     </body>"
          "   </html>";
    return out.toUtf8();
}

void MainWindow::on_viewExportPDFButton_clicked()
{
    _exportPDF->setHtml(scheduleToHml());
    _exportPDF->setHtmlPDF(scheduleToHmlForPDF());
    _exportPDF->show();
}

#if (QT_VERSION >= 0x050000) // Qt version 5 and above
    void MainWindow::myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString& msg)
#else // until version 5
    void MainWindow::myMessageHandler(QtMsgType type, const char* msg)
#endif
{
    QString dt = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
    QString txt = QString("[%1]\n").arg(dt);

    switch (type)
    {
    case QtDebugMsg:
        #if (QT_VERSION >= 0x050000) // Qt version 5 and above
            txt += QString("\t{Debug} ") + msg;
        #else // until version 5
            txt += QString("\t{Debug} %1").arg(msg);
        #endif
        break;
    case QtWarningMsg:
        #if (QT_VERSION >= 0x050000) // Qt version 5 and above
            txt += QString("\t{Warning} ") + msg;
        #else // until version 5
            txt += QString("\t{Warning} %1").arg(msg);
        #endif
        break;
    case QtCriticalMsg:
        #if (QT_VERSION >= 0x050000) // Qt version 5 and above
            txt += QString("\t{Critical} ") + msg;
        #else // until version 5
            txt += QString("\t{Critical} %1").arg(msg);
        #endif
        break;
    case QtFatalMsg:
        #if (QT_VERSION >= 0x050000) // Qt version 5 and above
            txt += QString("\t{Fatal} ") + msg;
        #else // until version 5
            txt += QString("\t{Fatal} %1").arg(msg);
        #endif
        abort();
        break;
    }

    QFile outFile(QDir(qApp->applicationDirPath()).path() + "/" + "opp.log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);

    QTextStream textStream(&outFile);
    textStream << txt << endl;

    LoggerSingleton::getInstance()->writeMessage(txt);
}

void MainWindow::playMire(QString fileName){
    if(_vlcMire == NULL){
        _vlcMire = libvlc_new(0, NULL);
        _mpMire = new MediaPlayer(_vlcMire,this);
        QString path = QString("mires")+QDir::separator()+fileName;
        _mireMire = new Media(path,_vlcMire);
        _pbMire = new Playback(_mireMire);
        _mpMire->setBackMode(MediaPlayer::NONE);
        _vWMire = new VideoWindow(this,VideoWindow::PROJECTION);
        _mpMire->setVideoView((VideoView*) _vWMire->videoWidget());
        _mpMire ->open(_pbMire);
        _mpMire->play();

        connect(_vWMire,SIGNAL(closed()),this,SLOT(closeMirePlayer()));
    }else{
        _mpMire->stop();
        _mpMire->close(_pbMire);
        QString path = QString("mires")+QDir::separator()+fileName;
        _mireMire = new Media(path,_vlcMire);
        _pbMire = new Playback(_mireMire);
        _mpMire ->open(_pbMire);
        _mpMire->play();
    }

}

void MainWindow::closeWindowTestPattern(){
    if(_vlcMire != NULL){
        _mpMire->stop();
        _mpMire->close(_pbMire);
        _vWMire->close();
    }
}

void MainWindow::closeMirePlayer(){
    _mpMire->stop();
    if(_vlcMire != NULL){
        libvlc_release(_vlcMire);
        _vlcMire=NULL;
    }
    if(_mpMire != NULL){
        delete(_mpMire);
        _mpMire=NULL;
    }if(_pbMire != NULL){
        delete(_pbMire);
        _pbMire=NULL;
    }if(_vWMire != NULL){
        delete(_vWMire);
        _vWMire = NULL;
    }
}

void MainWindow::on_actionTest_patterns_triggered()
{
    openDir("mires");
}

void MainWindow::on_actionScreenshots_triggered()
{
    openDir("screenshot");
}

void MainWindow::on_actionPlugins_triggered()
{
    openDir("pluginsOPP");
}

void MainWindow::on_helpAction_triggered()
{
    QDesktopServices::openUrl(QString("file:///") + qApp->applicationDirPath() + "/" +QString("help/usersDocumentation.pdf"));
}

void MainWindow::on_actionLog_triggered()
{
    QDesktopServices::openUrl(QString("file:///") + qApp->applicationDirPath() + "/" +QString("opp.log"));
}

void MainWindow::openDir(QString name){
    QString folder = QString("file:///") + qApp->applicationDirPath() + "/" +QString(name);
    QDir dir(folder);
    if(!dir.exists(folder)){
       dir.mkdir(folder);
    }
    QDesktopServices::openUrl(folder);
}

/**********************************************************************************\
                                        EVENTS
\**********************************************************************************/
void MainWindow::closeEvent (QCloseEvent *event)
{
    if(currentPlaylistModel()->isRunning()){
        if (1 == QMessageBox::warning(this, tr("Save"), tr("A projection is running, are you sure you want to close the software ?") ,tr("No"), tr("Yes"))){
            stop();
        }else{
            event->ignore();
        }
    }
    else
    {
        if(verifSave() == 0)
            event->ignore();
    }
}
