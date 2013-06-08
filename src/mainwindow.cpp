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
#include "mediaplayer.h"
#include "playback.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QList<QWidget*> lockedWidget;
    lockedWidget << ui->playlistsTabWidget;
    _locker = new Locker(lockedWidget, this);

    _lockSettingsWindow = new LockSettingsWindow(_locker, this);

    _videoWindow = new VideoWindow(this);

    // internal core initalization
    _app = new Application();
    _mediaPlayer = new MediaPlayer(_app->vlcInstance());
    _mediaPlayer->setVideoView( (VideoView*) _videoWindow->videoWidget() );

    _mediaListModel = new MediaListModel();
    _scheduleListModel = new ScheduleListModel();

    ui->binTableView->setModel(_mediaListModel);
    ui->scheduleTableView->setModel(_scheduleListModel);

    ui->timelineWidget->setMediaPlayer(_mediaPlayer);

    _statusWidget = new StatusWidget;
    ui->statusBar->addWidget(_statusWidget);
    connect(_mediaListModel, SIGNAL(mediaListChanged(int)), _statusWidget, SLOT(setMediaCount(int)));

    connect(ui->lockButton, SIGNAL(toggled(bool)), _locker, SLOT(toggle(bool)));

    // show/hide pannel actions
    connect(ui->quitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->resumeDetailsAction, SIGNAL(toggled(bool)), ui->projectTabWidget, SLOT(setVisible(bool)));
    connect(ui->binAction, SIGNAL(toggled(bool)), ui->binGroupBox, SLOT(setVisible(bool)));
    connect(ui->automationAction, SIGNAL(toggled(bool)), ui->scheduleGroupBox, SLOT(setVisible(bool)));
    connect(ui->statusBarAction, SIGNAL(toggled(bool)), ui->statusBar, SLOT(setVisible(bool)));

    connect(ui->newPlaylistAction, SIGNAL(triggered()), this, SLOT(createPlaylistTab()));

    // core connections
    connect(ui->playerVolumeSlider, SIGNAL(valueChanged(int)), _mediaPlayer, SLOT(setVolume(int)));
    connect(_mediaPlayer, SIGNAL(end()), ui->playerPlayButton, SLOT(toggle()));

    //DEBUG : this code add a media into the bin on launch
//    Media media("/Users/floomoon/Movies/3ours-OCPM.mkv", _app->vlcInstance());
//    _mediaListModel->addMedia(media);

    // set video mode actions data
    ui->actionProjection->setData(QVariant(VideoWindow::PROJECTION));
    ui->actionWindow->setData(QVariant(VideoWindow::WINDOW));

    // initialize from ui
    _mediaPlayer->setVolume(ui->playerVolumeSlider->value());

    // media settings input mapping
    _mediaSettingsMapper = new QDataWidgetMapper(this);
//    _mediaSettingsMapper->addMapping(ui->ratioComboBox, 2);

    // initialize media settings input
    ui->ratioComboBox->addItems(MediaSettings::ratioValues());
    ui->ratioComboBox->setAutoCompletion(true);

    connect(ui->ratioComboBox, SIGNAL(currentIndexChanged(int)), _mediaSettingsMapper, SLOT(submit()));

    createPlaylistTab();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _lockSettingsWindow;
    delete _mediaListModel;
    delete _mediaSettingsMapper;
    delete _videoWindow;
    delete _mediaPlayer;
    delete _app;
}

void MainWindow::initSettingsViews()
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
    QModelIndexList indexes = ui->binTableView->selectionModel()->selectedRows();

    foreach (QModelIndex index, indexes) {
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

void MainWindow::on_settingsAction_triggered()
{
    _settingsWindow = new SettingsWindow(this);
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

void MainWindow::on_playerPlayButton_clicked()
{
    if (_mediaPlayer->isPlaying()) {
        _mediaPlayer->pause();
    } else {
        if (_mediaPlayer->isPaused()) {
            _mediaPlayer->resume();
        } else {
            QModelIndexList indexes = currentPlaylistTableView()->selectionModel()->selectedRows();

            if (indexes.count() == 0) {
                ui->playerPlayButton->setChecked(false);
            } else {
                Playback *playback = currentPlaylistModel()->playlist().at(indexes.first().row());
                _mediaPlayer->open(playback);
                _mediaPlayer->play();
            }
        }
    }
}

void MainWindow::on_playerStopButton_clicked()
{
    if (_mediaPlayer->isPlaying() || _mediaPlayer->isPaused()) {
        _mediaPlayer->stop();
        ui->playerPlayButton->setChecked(false);
    }
}
void MainWindow::on_advancedSettingsButton_clicked()
{
    _advancedSettingsWindow = new AdvancedSettingsWindow(this);
    _advancedSettingsWindow->show();
    _advancedSettingsWindow->raise();
    _advancedSettingsWindow->activateWindow();
}

void MainWindow::on_advancedPictureSettingsButton_clicked()
{
    _advancedPictureSettingsWindow = new AdvancedPictureSettingsWindow(this);
    _advancedPictureSettingsWindow->show();
    _advancedPictureSettingsWindow->raise();
    _advancedPictureSettingsWindow->activateWindow();
}

void MainWindow::on_lockButton_clicked()
{
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

void MainWindow::createPlaylistTab()
{
    PlaylistTableView *newTab = new PlaylistTableView;
    PlaylistModel *newModel = new PlaylistModel(_mediaListModel);

    newTab->setModel(newModel);
    newTab->setSelectionBehavior(QAbstractItemView::SelectRows);
    newTab->horizontalHeader()->setStretchLastSection(true);

    int pos = ui->playlistsTabWidget->count();
    ui->playlistsTabWidget->addTab(newTab, "New playlist");

    ui->playlistsTabWidget->setCurrentWidget(newTab);
}

void MainWindow::on_playlistsTabWidget_tabCloseRequested(int index)
{
    if (ui->playlistsTabWidget->count() == 1) {
        createPlaylistTab();
    }

    delete (PlaylistModel*) ((PlaylistTableView*) ui->playlistsTabWidget->widget(index))->model();
    ui->playlistsTabWidget->removeTab(index);
}

void MainWindow::on_playlistsTabWidget_currentChanged(int index)
{
    PlaylistTableView *view = (PlaylistTableView*) ui->playlistsTabWidget->widget(index);
    PlaylistModel *model = (PlaylistModel*) view->model();

    disconnect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(updateSettings()));
    disconnect(view->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), _mediaSettingsMapper, SLOT(setCurrentModelIndex(QModelIndex)));

    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(updateSettings()));
    connect(view->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), _mediaSettingsMapper, SLOT(setCurrentModelIndex(QModelIndex)));

    _mediaSettingsMapper->setModel( model );

    _mediaSettingsMapper->clearMapping();
    _mediaSettingsMapper->addMapping(ui->ratioComboBox, 2);
}

void MainWindow::on_ratioComboBox_currentIndexChanged(int index)
{
//    Playback *playback = selectedPlayback();
//    if (playback) {
//        playback->mediaSettings()->setRatio((Ratio) index);
//    }
}

void MainWindow::on_subtitlesSyncSpinBox_valueChanged(double arg1)
{
    Playback *playback = selectedPlayback();
    if (playback) {
        playback->mediaSettings()->setSubtitlesSync(arg1);
    }
   //_mediaPlayer->setCurrentSubtitlesSync(arg1);
}

void MainWindow::on_gammaSpinBox_valueChanged(int arg1)
{
    Playback *playback = selectedPlayback();
    if (playback) {
        playback->mediaSettings()->setGamma(arg1);
    }
    // _mediaPlayer->setCurrentGamma(arg1);
}

void MainWindow::on_contrastSpinBox_valueChanged(int arg1)
{
    Playback *playback = selectedPlayback();
    if (playback) {
        playback->mediaSettings()->setContrast(arg1);
    }
    // _mediaPlayer->setCurrentContrast(arg1);
}

void MainWindow::on_brightnessSpinBox_valueChanged(int arg1)
{
    Playback *playback = selectedPlayback();
    if (playback) {
        playback->mediaSettings()->setBrightness(arg1);
    }
   // _mediaPlayer->setCurrentBrightness(arg1);
}

void MainWindow::on_saturationSpinBox_valueChanged(int arg1)
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
        playback->mediaSettings()->setHue(arg1);
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

void MainWindow::on_testPatternAction_triggered()
{
//    _mediaPlayer->play(Media(QFileDialog::getOpenFileName(this, tr("Open test pattern"), QDir::homePath(), tr("Media (*.avi *.mkv *.jpg *.png)")), _app->vlcInstance()));
}

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

    if (ok && !text.isEmpty())
        ui->playlistsTabWidget->setTabText(tabIndex, text);
}

Playback* MainWindow::selectedPlayback() const {
    QModelIndexList indexes = currentPlaylistTableView()->selectionModel()->selectedRows();

    if (indexes.count() == 0)
        return NULL;

    return currentPlaylistModel()->playlist().at(indexes.first().row());
}

PlaylistTableView* MainWindow::currentPlaylistTableView() const
{
    return (PlaylistTableView*) ui->playlistsTabWidget->currentWidget();
}

PlaylistModel* MainWindow::currentPlaylistModel() const
{
    return (PlaylistModel*) currentPlaylistTableView()->model();
}

void MainWindow::updateSettings()
{
    Playback* playback = selectedPlayback();
    if(!playback){
        ui->mediaSettingsWidget->setEnabled(false);
        return;
    }
    ui->mediaSettingsWidget->setEnabled(true);

    ui->audioTrackComboBox->clear();
    int listCount=playback->media()->audioTracks().count();
    for(int i=0;i<listCount;i++)
        ui->audioTrackComboBox->addItem(playback->media()->audioTracks().at(i).second,playback->media()->audioTracks().at(i).first);


    ui->videoTrackComboBox->clear();
    listCount=playback->media()->videoTracks().count();
    for(int i=0;i<listCount;i++)
        ui->videoTrackComboBox->addItem(playback->media()->videoTracks().at(i).second,playback->media()->videoTracks().at(i).first);

    ui->subtitlesTrackComboBox->clear();
    listCount=playback->media()->subtitlesTracks().count();
    for(int i=0;i<listCount;i++)
        ui->subtitlesTrackComboBox->addItem(playback->media()->subtitlesTracks().at(i).second,playback->media()->subtitlesTracks().at(i).first);

    ui->subtitlesSyncSpinBox->setValue(playback->mediaSettings()->subtitlesSync());

    qDebug()<<playback->mediaSettings()->gamma();
    ui->gammaSpinBox->setValue(playback->mediaSettings()->gamma());
    qDebug()<<playback->mediaSettings()->contrast();
    ui->contrastSpinBox->setValue(playback->mediaSettings()->contrast());
    qDebug()<<playback->mediaSettings()->brightness();
    ui->brightnessSpinBox->setValue(playback->mediaSettings()->brightness());
    qDebug()<<playback->mediaSettings()->saturation();
    ui->saturationSpinBox->setValue(playback->mediaSettings()->saturation());
    qDebug()<<playback->mediaSettings()->hue();
    ui->hueSpinBox->setValue(playback->mediaSettings()->hue());
}

void MainWindow::on_removePlaylistItemAction_triggered()
{
    QModelIndexList indexes = currentPlaylistTableView()->selectionModel()->selectedRows();

    if (indexes.count() == 0)
        return;

   currentPlaylistModel()->removePlayback(indexes.first().row());
}
