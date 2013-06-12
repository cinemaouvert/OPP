#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QHeaderView>
#include <QContextMenuEvent>
#include <QInputDialog>

#include "global.h"
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

#include "application.h"
#include "media.h"
#include "playlistplayer.h"
#include "mediaplayer.h"
#include "playback.h"
#include "utils.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QList<QWidget*> lockedWidget;
    lockedWidget << ui->playlistsTabWidget;
    _locker = new Locker(lockedWidget, this);

    connect(_locker, SIGNAL(toggled(bool)), ui->lockButton, SLOT(setChecked(bool)));
    connect(ui->lockButton, SIGNAL(clicked(bool)), _locker, SLOT(toggle(bool)));

    _lockSettingsWindow = new LockSettingsWindow(_locker, this);
    _videoWindow = new VideoWindow(this);

    // internal core initalization
    _app = new Application();
    _playlistPlayer = new PlaylistPlayer(_app->vlcInstance());

    _playlistPlayer->mediaPlayer()->setVideoView( (VideoView*) _videoWindow->videoWidget() );
    _playlistPlayer->mediaPlayer()->setVolume(ui->playerVolumeSlider->value());

    connect(ui->playerVolumeSlider, SIGNAL(valueChanged(int)), _playlistPlayer->mediaPlayer(), SLOT(setVolume(int)));
    connect(_playlistPlayer->mediaPlayer(), SIGNAL(playing(bool)), ui->playerPlayButton, SLOT(setChecked(bool)));
    connect(ui->playerStopButton, SIGNAL(clicked()), _playlistPlayer, SLOT(stop()));
    connect(ui->playerStopButton, SIGNAL(clicked(bool)), ui->playerPlayButton, SLOT(setChecked(bool)));
    connect(ui->playerPreviousButton, SIGNAL(clicked()), _playlistPlayer, SLOT(previous()));
    connect(ui->playerNextButton, SIGNAL(clicked()), _playlistPlayer, SLOT(next()));
    connect(_playlistPlayer, SIGNAL(end()), ui->playerPlayButton, SLOT(toggle()));

    _mediaListModel = new MediaListModel();
    _scheduleListModel = new ScheduleListModel();

    connect(ui->scheduleToggleEnabledButton, SIGNAL(toggled(bool)), _scheduleListModel, SLOT(toggleAutomation(bool)));

    ui->binTableView->setModel(_mediaListModel);
    ui->scheduleTableView->setModel(_scheduleListModel);
    ui->seekWidget->setMediaPlayer(_playlistPlayer->mediaPlayer());

    // ###########################################################################################
    ui->progEdit->setText("Nom de la programmation:    ");
    ui->nbrfilmlabel->setText("Nombre de films:  "+QString::number(0)+"                   Nombre images:   "+QString::number(0));
    ui->durelabel_2->setText("Duree totale: "+msecToQTime(0).toString("hh:mm:ss"));
    ui->noteEdit->setText("Note(s):     ");

    connect(_mediaListModel, SIGNAL(mediaListChanged(int)),this, SLOT(setSummary(int)));

    ui->audiolabel->setText("Codecs audio:  ");
    ui->videolabel->setText("Codecs video:  ");
    ui->canauxaudiolabel->setText("Canaux audio:  ");
    ui->formatlabel->setText("Formats images:  ");

    connect(_mediaListModel, SIGNAL(mediaListChanged(int)),this, SLOT(setDetails(int)));
   //##############################################################################################
    _statusWidget = new StatusWidget;
    ui->statusBar->addWidget(_statusWidget);
    connect(_mediaListModel, SIGNAL(mediaListChanged(int)), _statusWidget, SLOT(setMediaCount(int)));

    // show/hide pannel actions
    connect(ui->quitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->resumeDetailsAction, SIGNAL(toggled(bool)), ui->projectTabWidget, SLOT(setVisible(bool)));
    connect(ui->binAction, SIGNAL(toggled(bool)), ui->binGroupBox, SLOT(setVisible(bool)));
    connect(ui->automationAction, SIGNAL(toggled(bool)), ui->scheduleGroupBox, SLOT(setVisible(bool)));
    connect(ui->statusBarAction, SIGNAL(toggled(bool)), ui->statusBar, SLOT(setVisible(bool)));

    connect(ui->newPlaylistAction, SIGNAL(triggered()), this, SLOT(createPlaylistTab()));

//    DEBUG : this code add a media into the bin on launch
    //_mediaListModel->addMedia(new Media("/Users/floomoon/Movies/3ours-OCPM.mkv", _app->vlcInstance()));

    // set video mode actions data
    ui->actionProjection->setData(QVariant(VideoWindow::PROJECTION));
    ui->actionWindow->setData(QVariant(VideoWindow::WINDOW));

    // media settings input mapping
    _mediaSettingsMapper = new QDataWidgetMapper(this);

    // initialize media settings input
//    ui->ratioComboBox->addItems(MediaSettings::ratioValues());
//    ui->ratioComboBox->setAutoCompletion(true);

//    connect(ui->ratioComboBox, SIGNAL(currentIndexChanged(int)), _mediaSettingsMapper, SLOT(submit()));

    createPlaylistTab();

    ui->scheduleLaunchAtDateEdit->setDate(QDate::currentDate());
    ui->scheduleLaunchAtTimeEdit->setTime(QTime::currentTime());

    ui->ratioComboBox->clear();
    ui->ratioComboBox->addItems(MediaSettings::ratioValues());

    _advancedSettingsWindow = new AdvancedSettingsWindow(this);
    _advancedPictureSettingsWindow = new AdvancedPictureSettingsWindow(this);
    _settingsWindow = new SettingsWindow(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _lockSettingsWindow;
    delete _mediaListModel;
    delete _mediaSettingsMapper;
    delete _videoWindow;
    delete _playlistPlayer;
    delete _app;
}

void MainWindow::setSummary(int count)
{
    ui->nbrfilmlabel->setText("Nombre de films:   "+QString::number(_mediaListModel->filmsNumber())+
                              "Nombre images:     "+QString::number( _mediaListModel->imageNumber()));
    ui->durelabel_2->setText("Duree totale: "+_mediaListModel->summaryTotalDuration().toString("hh:mm:ss"));
}

void MainWindow::setDetails(int count)
{


}



void MainWindow::on_binAddMediaButton_clicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("New media"), QDir::homePath(), tr("Media (*.avi *.mkv *.jpg *.png)"));

    foreach (QString fileName, fileNames) {
        Media *media = new Media(fileName, _app->vlcInstance());
        if (media->exists() == false) {
            QMessageBox::warning(this, "Import media", QString("The file %1 does not exist. Maybe it was deleted.").arg(media->location()));
            delete media;
        } else if (_mediaListModel->addMedia(media) == false) {
            QMessageBox::warning(this, "Import media", QString("The file %1 was already imported.").arg(media->location()));
            delete media;
        }
    }
}

void MainWindow::on_binDeleteMediaButton_clicked()
{
    QItemSelectionModel *selectionModel = ui->binTableView->selectionModel();

    while (selectionModel->selectedRows().count() != 0) {
        QModelIndex index = selectionModel->selectedRows().first();
        Media *media = _mediaListModel->mediaList().at(index.row());

        if (media->isUsed()) {
            if (1 == QMessageBox::warning(this, "remove media", "This media is used. All references of this media into playlists will be deleted too.\n Are you sure to remove this media ?" ,"No", "Yes"))
            {
                int countPlaylists = ui->playlistsTabWidget->count();
                for (int i = 0; i < countPlaylists; i++) {
                    PlaylistModel *model = (PlaylistModel*) ((PlaylistTableView*) ui->playlistsTabWidget->widget(i))->model();
                    model->removePlaybackWithDeps(media);
                }
            }
        }

        _mediaListModel->removeMedia(index.row());
    }
}

/***********************************************************************************************\
                                          Playback Settings
\***********************************************************************************************/

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
    _lockSettingsWindow->show();
    _lockSettingsWindow->raise();
    _lockSettingsWindow->activateWindow();
}

void MainWindow::on_audioTrackComboBox_currentIndexChanged(int index)
{
    if (index == -1)
        return;

    Playback *playback = selectedPlayback();
    if(playback)
    {
        const int track = playback->media()->audioTracks().at(index);
        playback->mediaSettings()->setAudioTrack(track);
    }

    emit SIGNAL(layoutChanged());
}

void MainWindow::on_videoTrackComboBox_currentIndexChanged(int index)
{
    if (index == -1)
        return;

    Playback *playback = selectedPlayback();
    if(playback)
    {
        const int track = playback->media()->videoTracks().at(index);
        playback->mediaSettings()->setVideoTrack(track);
    }

//    emit currentPlaylistModel()->layoutChanged();
}

void MainWindow::on_subtitlesTrackComboBox_currentIndexChanged(int index)
{
    if (index == -1)
        return;
    qDebug() << "INDEX = "<<index;
    Playback *playback = selectedPlayback();
    if(playback)
    {
        //const int track = playback->media()->subtitlesTracks().at(index);
        playback->mediaSettings()->setSubtitlesTrack(index);
    }

//    emit currentPlaylistModel()->layoutChanged();
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
   //_mediaPlayer->setCurrentSubtitlesSync(arg1);
}

void MainWindow::on_gammaSpinBox_valueChanged(double arg1)
{
    Playback *playback = selectedPlayback();
    if (playback) {
        playback->mediaSettings()->setGamma(arg1);
    }
    // _mediaPlayer->setCurrentGamma(arg1);
}

void MainWindow::on_contrastSpinBox_valueChanged(double arg1)
{
    Playback *playback = selectedPlayback();
    if (playback) {
        playback->mediaSettings()->setContrast(arg1);
    }
    // _mediaPlayer->setCurrentContrast(arg1);
}

void MainWindow::on_brightnessSpinBox_valueChanged(double arg1)
{
    Playback *playback = selectedPlayback();
    if (playback) {
        playback->mediaSettings()->setBrightness(arg1);
    }
   // _mediaPlayer->setCurrentBrightness(arg1);
}

void MainWindow::on_saturationSpinBox_valueChanged(double arg1)
{
    Playback *playback = selectedPlayback();
    if (playback) {
        playback->mediaSettings()->setSaturation(arg1);
    }
    // _mediaPlayer->setCurrentSaturation(arg1);
}

void MainWindow::on_hueSpinBox_valueChanged(int arg1)
{
    Playback *playback = selectedPlayback();
    if (playback) {
        playback->mediaSettings()->setHue((int)arg1);
    }
    // _mediaPlayer->setCurrentHue(arg1);
}

void MainWindow::on_audioSyncDoubleSpinBox_valueChanged(double arg1)
{
    Playback *playback = selectedPlayback();
    if (playback) {
        playback->mediaSettings()->setAudioSync(arg1);
    }
}

void MainWindow::updateSettings()
{
    Playback* playback = selectedPlayback();
    if(!playback){
        ui->mediaSettingsWidget->setEnabled(false);
        return;
    }

    ui->audioTrackComboBox->blockSignals(true);
    ui->videoTrackComboBox->blockSignals(true);
    ui->subtitlesTrackComboBox->blockSignals(true);

    ui->mediaSettingsWidget->setEnabled(true);

    QString item="";
    ui->audioTrackComboBox->clear();
    int listCount=playback->media()->audioTracks().count();
    for(int i=0;i<listCount;i++)
    {
        if(playback->media()->audioTracks().at(i)==-1)
            item="Disabled";
        else
        {
            item="Track ";
            item+=QString::number(playback->media()->audioTracks().at(i));
        }
        ui->audioTrackComboBox->addItem(item);
    }

    ui->videoTrackComboBox->clear();
    listCount=playback->media()->videoTracks().count();
    for(int i=0;i<listCount;i++)
    {
        if(playback->media()->videoTracks().at(i)==-1)
            item="Disabled";
        else
        {
            item="Track ";
            item+=QString::number(playback->media()->videoTracks().at(i));
        }
        ui->videoTrackComboBox->addItem(item);
    }

    ui->subtitlesTrackComboBox->clear();
    listCount=playback->media()->subtitlesTracks().count();

    for(int i=0;i<listCount;i++)
    {
        if(playback->media()->subtitlesTracks().at(i)==-1)
            item="Disabled";
        else
        {
            item="Track ";
            item+=QString::number(playback->media()->subtitlesTracks().at(i));
        }
        ui->subtitlesTrackComboBox->addItem(item);
    }

    ui->subtitlesSyncSpinBox->setValue(playback->mediaSettings()->subtitlesSync());

    ui->gammaSpinBox->setValue(playback->mediaSettings()->gamma());
    ui->contrastSpinBox->setValue(playback->mediaSettings()->contrast());
    ui->brightnessSpinBox->setValue(playback->mediaSettings()->brightness());
    ui->saturationSpinBox->setValue(playback->mediaSettings()->saturation());
    ui->hueSpinBox->setValue(playback->mediaSettings()->hue());

    ui->audioTrackComboBox->setCurrentIndex(getTrackIndex(playback->media()->audioTracks(), playback->mediaSettings()->audioTrack()));
    ui->videoTrackComboBox->setCurrentIndex(getTrackIndex(playback->media()->videoTracks(), playback->mediaSettings()->videoTrack()));
    ui->subtitlesTrackComboBox->setCurrentIndex(playback->mediaSettings()->subtitlesTrack());

    ui->ratioComboBox->setCurrentIndex(playback->mediaSettings()->ratio());

    ui->audioTrackComboBox->blockSignals(false);
    ui->videoTrackComboBox->blockSignals(false);
    ui->subtitlesTrackComboBox->blockSignals(false);
}

/*Returns a track index in a combo box*/
int MainWindow::getTrackIndex(QList<int> list, int track)
{
    for(int i=0;i<list.count();i++)
    {
        if(list.at(i)==track)
            return i;
    }
    return 0;
}

/***********************************************************************************************\
                                          Player
\***********************************************************************************************/

void MainWindow::on_playerPlayButton_clicked(bool checked)
{
    PlaylistModel *playlistModel = currentPlaylistModel();
    if (checked) {
        if (_playlistPlayer->mediaPlayer()->isPaused()) {
            _playlistPlayer->mediaPlayer()->resume();
        } else {
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
            _videoWindow->setDisplayMode( (VideoWindow::DisplayMode) action->data().toInt() );
        }
    }
}

void MainWindow::on_testPatternAction_triggered()
{
//    _mediaPlayer->play(Media(QFileDialog::getOpenFileName(this, tr("Open test pattern"), QDir::homePath(), tr("Media (*.avi *.mkv *.jpg *.png)")), _app->vlcInstance()));
}

/***********************************************************************************************\
                                          Playlist
\***********************************************************************************************/

void MainWindow::createPlaylistTab()
{
    PlaylistTableView *newTab = new PlaylistTableView;
    Playlist *playlist = new Playlist(_app->vlcInstance(), "new playlist");
    PlaylistModel *newModel = new PlaylistModel(playlist, _mediaListModel);

    connect(playlist, SIGNAL(titleChanged()), _scheduleListModel, SIGNAL(layoutChanged()));

    newTab->setModel(newModel);
    newTab->setSelectionBehavior(QAbstractItemView::SelectRows);
    newTab->horizontalHeader()->setStretchLastSection(true);

    ui->playlistsTabWidget->addTab(newTab, playlist->title());
    ui->playlistsTabWidget->setCurrentWidget(newTab);

    updatePlaylistListCombox();
}

void MainWindow::on_playlistsTabWidget_tabCloseRequested(int index)
{
    Playlist *playlist = playlistAt(index);

    // delete schedule which use the playlist
    if (_scheduleListModel->isScheduled(playlist)) {
        if (0 == QMessageBox::warning(this, "remove playlist", "This playlist was scheduled. All schedules which use this playlist will be deleted too.\n Are you sure to remove this playlist ?" ,"No", "Yes"))
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
}

void MainWindow::on_playlistsTabWidget_currentChanged(int index)
{
    PlaylistTableView *view = (PlaylistTableView*) ui->playlistsTabWidget->widget(index);
    PlaylistModel *model = (PlaylistModel*) view->model();

    // TODO : disconnect when the index has not changed yet (to get the old selected index)
//    disconnect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(updateSettings()));
//    disconnect(view->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), _mediaSettingsMapper, SLOT(setCurrentModelIndex(QModelIndex)));

    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(updateSettings()));
    connect(view->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), _mediaSettingsMapper, SLOT(setCurrentModelIndex(QModelIndex)));

    connect(_playlistPlayer->mediaPlayer(), SIGNAL(playing()), model, SLOT(playItem()));
    connect(_playlistPlayer->mediaPlayer(), SIGNAL(paused()), model, SLOT(pauseItem()));
    connect(_playlistPlayer->mediaPlayer(), SIGNAL(stopped()), model, SLOT(stopItem()));

    connect(_playlistPlayer, SIGNAL(itemChanged(int)), model, SLOT(setPlayingItem(int)));

    _mediaSettingsMapper->setModel( model );


    _mediaSettingsMapper->clearMapping();
//    _mediaSettingsMapper->addMapping(ui->ratioComboBox, 2);
//    _mediaSettingsMapper->addMapping(ui->subtitlesTrackComboBox, 4);

    QModelIndexList indexes = view->selectionModel()->selectedIndexes();
    if(indexes.count() > 0)
        _mediaSettingsMapper->setCurrentModelIndex(indexes.first());

    updateSettings();

    _playlistPlayer->setPlaylist(model->playlist());
}

void MainWindow::on_renamePlaylistAction_triggered()
{
    int tabIndex = ui->playlistsTabWidget->currentIndex();
    bool ok;

    QString text = QInputDialog::getText(this,
        tr("Rename playlist"),
        tr("Playlist title :"),
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

void MainWindow::on_removePlaylistItemAction_triggered()
{
    QModelIndexList indexes = currentPlaylistTableView()->selectionModel()->selectedRows();

    if (indexes.count() == 0)
        return;

   currentPlaylistModel()->removePlayback(indexes.first().row());
   updateSettings();
}

/***********************************************************************************************\
                                          Project import/export
\***********************************************************************************************/

void MainWindow::on_saveAsAction_triggered()
{
    /* Bin save */

    //on sauvegarde le fichier où on veut
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Project"), "", tr("OPP (*.opp);;All Files (*)"));
    if (fileName.isEmpty())
         return;
     else {
         QFile file(fileName);
         if (!file.open(QIODevice::WriteOnly)) {
             QMessageBox::information(this, tr("Unable to open file"),file.errorString());
         }
         //save data into the file
         QDataStream out(&file);
         out << _mediaListModel->mediaList();
    }
}

void MainWindow::on_openListingAction_triggered()
{
    /* Bin Loading */

    // Go seek the file to load
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Project"), "",tr("OPP (*.opp);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    else{
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)){
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }

        //reset the list once the file is selected
        while(_mediaListModel->rowCount() > 0) {
            _mediaListModel->removeRows(0, 1);
        }

        //load data from the file, and add media in _mediaListModel
        QDataStream in(&file);
        int i=0;
        while(!in.atEnd()){
            QString loc;
            in >> loc;
            _mediaListModel->addMedia(new Media(loc, _app->vlcInstance()));
            i++;
        }
    }//end else
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
        QMessageBox::critical(this, "Schedule validation", QString("The schedule launch date must be later than the current date."));
        return;
    }

    Playlist *playlist = playlistAt(playlistIndex);
    Schedule *schedule = new Schedule(playlist, launchAt);

    if (_scheduleListModel->isSchedulable(schedule)) {
        connect(schedule, SIGNAL(triggered(Playlist*)), _playlistPlayer, SLOT(playPlaylist(Playlist*)));
        _scheduleListModel->addSchedule(schedule);
    } else {
        QMessageBox::critical(this, "Schedule validation", QString("A playlist was already scheduled between the %1 and %2, \nPlease choose an other launch date.")
                                .arg(schedule->launchAt().toString())
                                .arg(schedule->finishAt().toString())
                              );
        delete schedule;
    }
}

void MainWindow::on_scheduleDelayButton_clicked()
{
    int delay = ui->scheduleDelaySpinBox->value() /*min*/;

    _scheduleListModel->delayAll(delay * 60 * 1000 /*ms*/);
}

void MainWindow::on_scheduleToggleEnabledButton_toggled(bool checked)
{
    if (checked) {
        ui->scheduleToggleEnabledButton->setText("Stop automation");
    } else {
        ui->scheduleToggleEnabledButton->setText("Start automation");
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
