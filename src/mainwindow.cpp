#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

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

#include "application.h"
#include "media.h"
#include "mediaplayer.h"
#include "playback.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _videoWindow = new VideoWindow(this);

    _app = new Application();
    _mediaPlayer = new MediaPlayer(_app->vlcInstance());
    _mediaPlayer->setVideoView( (VideoView*) _videoWindow->videoWidget() );

    _mediaListModel = new MediaListModel();

    ui->binTableView->setModel(_mediaListModel);
    ui->timelineWidget->setMediaPlayer(_mediaPlayer);

    // show/hide pannel actions
    connect(ui->quitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->resumeDetailsAction, SIGNAL(toggled(bool)), ui->projectTabWidget, SLOT(setVisible(bool)));
    connect(ui->binAction, SIGNAL(toggled(bool)), ui->binGroupBox, SLOT(setVisible(bool)));
    connect(ui->automationAction, SIGNAL(toggled(bool)), ui->scheduleGroupBox, SLOT(setVisible(bool)));
    connect(ui->statusBarAction, SIGNAL(toggled(bool)), ui->statusBar, SLOT(setVisible(bool)));

    // core connections
    connect(ui->playerVolumeSlider, SIGNAL(valueChanged(int)), _mediaPlayer, SLOT(setVolume(int)));

    //DEBUG : this code add a media into the bin on launch
//    Media media("/Users/floomoon/Movies/3ours-OCPM.mkv", _app->vlcInstance());
//    _mediaListModel->addMedia(media);
//    qDebug()<<_videoWindow->videoWidget()->request();

    // set video mode actions data
    ui->actionProjection->setData(QVariant(VideoWindow::PROJECTION));
    ui->actionWindow->setData(QVariant(VideoWindow::WINDOW));

    // initialize from ui
    _mediaPlayer->setVolume(ui->playerVolumeSlider->value());

    ui->ratioComboBox->addItems(MediaSettings::ratioValues());
    ui->ratioComboBox->removeItem(0);
    // TODO : select default

    initSettingsViews();
    createPlaylistTab();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _mediaListModel;
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
        Media media(fileName, _app->vlcInstance());
        if (media.exists() == false) {
            QMessageBox::warning(this, "Import media", QString("The file %1 does not exist. Maybe it was deleted.").arg(media.location()));
        } else if (_mediaListModel->addMedia(media) == false) {
            QMessageBox::warning(this, "Import media", QString("The file %1 was already imported.").arg(media.location()));
        }
    }
}

void MainWindow::on_binDeleteMediaButton_clicked()
{
    QModelIndexList indexes = ui->binTableView->selectionModel()->selectedRows();
    foreach (QModelIndex index, indexes) {
        _mediaListModel->removeRows(index.row(), 1);
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
    lockSettingsWindow = new LockSettingsWindow(this);
    lockSettingsWindow->show();
    lockSettingsWindow->raise();
    lockSettingsWindow->activateWindow();
}

void MainWindow::on_playerPlayButton_clicked()
{
    if (_mediaPlayer->isPlaying()) {
        _mediaPlayer->pause();
    } else {
        if (_mediaPlayer->isPaused()) {
            _mediaPlayer->resume();
        } else {
            PlaylistTableView *currentPlaylistView = (PlaylistTableView*) ui->playlistsTabWidget->currentWidget();
            PlaylistModel *currentPlaylistModel = (PlaylistModel*) currentPlaylistView->model();
            QModelIndexList indexes = currentPlaylistView->selectionModel()->selectedRows();

            if (indexes.count() == 0) {
                ui->playerPlayButton->toggle(); // display play icon
            } else {
                Playback *playback = currentPlaylistModel->playlist().at(indexes.first().row());
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
    lockSettingsWindow = new LockSettingsWindow(this);
    lockSettingsWindow->show();
    lockSettingsWindow->raise();
    lockSettingsWindow->activateWindow();
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

PlaylistTableView* MainWindow::createPlaylistTab()
{
    PlaylistTableView *newTab = new PlaylistTableView;
    newTab->setModel(new PlaylistModel(_mediaListModel));
    newTab->setSelectionBehavior(QAbstractItemView::SelectRows);

    int pos = ui->playlistsTabWidget->count() - 1;
    ui->playlistsTabWidget->insertTab(pos, newTab, "New playlist");

    ui->playlistsTabWidget->setCurrentWidget(newTab);

    return newTab;
}

void MainWindow::on_playlistsTabWidget_currentChanged(int index)
{
    if (index == ui->playlistsTabWidget->count()-1) {
        createPlaylistTab();
    } else {

    }
}

void MainWindow::on_ratioComboBox_currentIndexChanged(int index)
{
    _mediaPlayer->setCurrentRatio((Ratio) index);
}
