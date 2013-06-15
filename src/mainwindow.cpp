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
#include "datastorage.h"

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

    /**
     * @author Hamza Haddar
     */
    // ###########################################################################################
    ui->nbrfilmlabel->setText(tr("Number of movies : ")+QString::number(0)+tr(" - Number of pictures : ")+QString::number(0));
    ui->durelabel_2->setText(tr("Total duration: ")+msecToQTime(0).toString("hh:mm:ss"));

    connect(_mediaListModel, SIGNAL(mediaListChanged(int)),this, SLOT(setSummary(int)));

    ui->audiolabel->setText(tr("Audio codecs: "));
    ui->videolabel->setText(tr("Video codecs: "));
    ui->canauxaudiolabel->setText(tr("Audio channels: "));
    ui->formatlabel->setText(tr("Picture formats: "));

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

    // set video mode actions data
    ui->actionProjection->setData(QVariant(VideoWindow::PROJECTION));
    ui->actionWindow->setData(QVariant(VideoWindow::WINDOW));

    _dataStorage = new DataStorage(_app, this/*FIX : ref 0000001*/);
    _dataStorage->setMediaListModel(_mediaListModel);
    _dataStorage->setScheduleListModel(_scheduleListModel);

    connect(ui->progEdit,SIGNAL(textChanged(QString)), _dataStorage, SLOT(setProjectTitle(QString)));

    createPlaylistTab();

    ui->scheduleLaunchAtDateEdit->setDate(QDate::currentDate());
    ui->scheduleLaunchAtTimeEdit->setTime(QTime::currentTime());

    ui->ratioComboBox->clear();
    ui->ratioComboBox->addItems(MediaSettings::ratioValues());

    _advancedSettingsWindow = new AdvancedSettingsWindow(this);
    _advancedPictureSettingsWindow = new AdvancedPictureSettingsWindow(this);
    _settingsWindow = new SettingsWindow(this);

    ui->scheduleToggleEnabledButton->click();

    currentPlaylistTableView()->setDragDropMode(QAbstractItemView::InternalMove);
    currentPlaylistTableView()->setDragEnabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _lockSettingsWindow;
    delete _mediaListModel;
    delete _videoWindow;
    delete _playlistPlayer;
    delete _dataStorage;
    delete _app;
}

// FIX : ref 0000001
QTabWidget* MainWindow::playlistTabWidget() const { return ui->playlistsTabWidget; }


/***********************************************************************************************\
                                          Project Details/Summary
\***********************************************************************************************/

/**
 * @author Hamza Haddar
 */
void MainWindow::setSummary(int count)
{
    ui->nbrfilmlabel->setText(tr("Number of movies : ")+QString::number(_mediaListModel->filmsNumber())+
                              tr(" - Number of pictures : ")+QString::number( _mediaListModel->imageNumber()));
    ui->durelabel_2->setText(tr("Total duration : ")+_mediaListModel->summaryTotalDuration().toString("hh:mm:ss"));
}

/**
 * @author Hamza Haddar
 */
void MainWindow::setDetails(int count)
{


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
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("New media"), QDir::homePath(), tr("Media (%1)").arg(Media::mediaExtensions().join(" ")));

    foreach (QString fileName, fileNames) {
        Media *media = new Media(fileName, _app->vlcInstance());
        if (media->exists() == false) {
            QMessageBox::warning(this, tr("Import media"), QString(tr("The file %1 does not exist. Maybe it was deleted.")).arg(media->location()));
            delete media;
        } else if (_mediaListModel->addMedia(media) == false) {
            QMessageBox::warning(this, tr("Import media"), QString(tr("The file %1 was already imported.")).arg(media->location()));
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
            if (1 == QMessageBox::warning(this, tr("Remove media"), tr("This media is used. All references of this media into playlists will be deleted too.\n Are you sure to remove this media ?") ,tr("No"), tr("Yes")))
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
                                     Playback Settings Management
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
        ui->playerControlsWidget->setEnabled(false);
        return;
    }

    ui->audioTrackComboBox->blockSignals(true);
    ui->videoTrackComboBox->blockSignals(true);
    ui->subtitlesTrackComboBox->blockSignals(true);

    ui->mediaSettingsWidget->setEnabled(true);
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

    ui->audioTrackComboBox->blockSignals(false);
    ui->videoTrackComboBox->blockSignals(false);
    ui->subtitlesTrackComboBox->blockSignals(false);
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

// TODO : play test pattern
void MainWindow::on_testPatternAction_triggered()
{
}


/***********************************************************************************************\
                                          Playlist
\***********************************************************************************************/


void MainWindow::createPlaylistTab()
{
    PlaylistTableView *newTab = new PlaylistTableView;
    Playlist *playlist = new Playlist(_app->vlcInstance(), tr("New playlist"));
    PlaylistModel *newModel = new PlaylistModel(playlist, _mediaListModel, _scheduleListModel);

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
}

void MainWindow::on_playlistsTabWidget_currentChanged(int index)
{
    PlaylistTableView *view = (PlaylistTableView*) ui->playlistsTabWidget->widget(index);
    PlaylistModel *model = (PlaylistModel*) view->model();

    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(updateSettings()));

    connect(_playlistPlayer->mediaPlayer(), SIGNAL(playing()), model, SLOT(playItem()));
    connect(_playlistPlayer->mediaPlayer(), SIGNAL(paused()), model, SLOT(pauseItem()));
    connect(_playlistPlayer->mediaPlayer(), SIGNAL(stopped()), model, SLOT(stopItem()));
    connect(_playlistPlayer, SIGNAL(itemChanged(int)), model, SLOT(setPlayingItem(int)));

    updateSettings();

    _playlistPlayer->setPlaylist(model->playlist());
}

void MainWindow::on_renamePlaylistAction_triggered()
{
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

void MainWindow::on_removePlaylistItemAction_triggered()
{
    QModelIndexList indexes = currentPlaylistTableView()->selectionModel()->selectedRows();

    if (indexes.count() == 0)
        return;

   currentPlaylistModel()->removePlayback(indexes.first().row());
   updateSettings();
   _scheduleListModel->updateLayout();
}

// FIX : ref 0000001
void MainWindow::restorePlaylistTab(PlaylistModel *model)
{
    PlaylistTableView *newTab = new PlaylistTableView;

    connect(model->playlist(), SIGNAL(titleChanged()), _scheduleListModel, SIGNAL(layoutChanged()));

    newTab->setModel(model);
    newTab->setSelectionBehavior(QAbstractItemView::SelectRows);
    newTab->horizontalHeader()->setStretchLastSection(true);

    ui->playlistsTabWidget->addTab(newTab, model->playlist()->title());
    ui->playlistsTabWidget->setCurrentWidget(newTab);

    updatePlaylistListCombox();
}


/***********************************************************************************************\
                                          Project import/export
\***********************************************************************************************/


void MainWindow::on_saveAsAction_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Project"), "", tr("OPP file (*.opp)"));

    if (fileName.isEmpty()) {
         return;
    } else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),file.errorString());
        }

        for (int i = 0; i < ui->playlistsTabWidget->count(); i++)
            _dataStorage->addPlaylistModel((PlaylistModel*) ( (PlaylistTableView*) ui->playlistsTabWidget->widget(i) )->model());

        _dataStorage->save(file);
        file.close();
    }
}


void MainWindow::on_openListingAction_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"), "", tr("OPP file (*.opp)"));

    if (fileName.isEmpty()) {
         return;
    } else {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadWrite)) {
            QMessageBox::information(this, tr("Unable to open file"),file.errorString());
        }

        _dataStorage->load(file);

        file.close();

        ui->progEdit->setText(_dataStorage->projectTitle());
        ui->notesEdit->setText(_dataStorage->projectNotes());
    }
}


void MainWindow::on_newListingAction_triggered()
{
    _dataStorage->clear();

    // FIX : ref 0000001
    // add empty tab and remove all other one (init state)
    createPlaylistTab();
    while (ui->playlistsTabWidget->count() > 1)
        ui->playlistsTabWidget->removeTab(0);

    ui->progEdit->clear();
    ui->notesEdit->clear();
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

    _scheduleListModel->delayAll(delay * 60 * 1000 /*ms*/);
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

void MainWindow::on_playlistUpButton_clicked()
{
    QModelIndexList indexes = currentPlaylistTableView()->selectionModel()->selectedRows();
    if(indexes.count()==0)
        return;

    if(currentPlaylistModel()->moveUp(indexes.first()))
        currentPlaylistTableView()->setCurrentIndex(currentPlaylistModel()->index(indexes.first().row() - 1, indexes.first().column()));
}

void MainWindow::on_playlistDownButton_clicked()
{
    QModelIndexList indexes = currentPlaylistTableView()->selectionModel()->selectedRows();

    if(indexes.count()==0)
        return;
    if(currentPlaylistModel()->moveDown(indexes.first()))
        currentPlaylistTableView()->setCurrentIndex(currentPlaylistModel()->index(indexes.first().row() + 1, indexes.first().column()));
}
