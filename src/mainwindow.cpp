#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

#include "settingswindow.h"
#include "locksettingswindow.h"
#include "medialistmodel.h"

#include "application.h"
#include "media.h"
#include "mediaplayer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _app = new Application();
    _mediaPlayer = new MediaPlayer(_app->vlcInstance());

    _mediaListModel = new MediaListModel();
    ui->binTableView->setModel(_mediaListModel);

    // show/hide pannel actions
    connect(ui->quitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->resumeDetailsAction, SIGNAL(toggled(bool)), ui->projectTabWidget, SLOT(setVisible(bool)));
    connect(ui->binAction, SIGNAL(toggled(bool)), ui->binGroupBox, SLOT(setVisible(bool)));
    connect(ui->automationAction, SIGNAL(toggled(bool)), ui->scheduleGroupBox, SLOT(setVisible(bool)));
    connect(ui->statusBarAction, SIGNAL(toggled(bool)), ui->statusBar, SLOT(setVisible(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _mediaListModel;
    delete _mediaPlayer;
    delete _app;
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
            // temporaly get selected media from the bin
            // TODO : get media of the selected playback from _playlistModel ...
            QModelIndexList indexes = ui->binTableView->selectionModel()->selectedRows();

            if (indexes.count() == 0) {
                ui->playerPlayButton->toggle(); // display play icon
            } else {
                _mediaPlayer->open(_mediaListModel->mediaList().at(indexes.first().row()));
                _mediaPlayer->play();
            }
        }
    }
}

void MainWindow::on_playerStopButton_clicked()
{
    if (_mediaPlayer->isPlaying() || _mediaPlayer->isPaused()) {
        _mediaPlayer->stop();
        ui->playerPlayButton->toggle();
    }
}
