#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class SettingsWindow;
class LockSettingsWindow;

class MediaListModel;

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

    /* player controls */
    void on_playerPlayButton_clicked();
    void on_playerStopButton_clicked();

private:
    Ui::MainWindow *ui;

    SettingsWindow *_settingsWindow;
    LockSettingsWindow *lockSettingsWindow;

    Application *_app;
    MediaPlayer *_mediaPlayer;

    MediaListModel *_mediaListModel;
};

#endif // MAINWINDOW_H
