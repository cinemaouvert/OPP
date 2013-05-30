#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class VideoWindow;
class SettingsWindow;
class LockSettingsWindow;
class AdvancedSettingsWindow;
class AdvancedPictureSettingsWindow;

class MediaListModel;
class PlaylistModel;

class Application;
class MediaPlayer;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    /* bin events */
    void on_binAddMediaButton_clicked();
    void on_binDeleteMediaButton_clicked();

    /* action bar events */
    void on_settingsAction_triggered();
    void on_lockSettingsAction_triggered();

    /*parameters events*/
    void on_advancedSettingsButton_clicked();
    void on_advancedPictureSettingsButton_clicked();
    void on_lockButton_clicked();

    /* player controls */
    void on_playerPlayButton_clicked();
    void on_playerStopButton_clicked();


private:
    Ui::MainWindow *ui;
    VideoWindow *_videoWindow;

    SettingsWindow *_settingsWindow;
    LockSettingsWindow *lockSettingsWindow;
    AdvancedSettingsWindow *_advancedSettingsWindow;
    AdvancedPictureSettingsWindow *_advancedPictureSettingsWindow;

    Application *_app;
    MediaPlayer *_mediaPlayer;

    MediaListModel *_mediaListModel;
    PlaylistModel *_playlistModel;
};

#endif // MAINWINDOW_H
