#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDataWidgetMapper>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class VideoWindow;
class SettingsWindow;
class LockSettingsWindow;
class AdvancedSettingsWindow;
class AdvancedPictureSettingsWindow;
class PlaylistTableView;
class StatusWidget;

class PlaylistModel;
class MediaListModel;
class ScheduleListModel;

class Application;
class PlaylistPlayer;
class Playback;
class Locker;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void initSettingsViews();
    void on_ratioComboBox_currentIndexChanged(int index);

    /* bin events */
    void on_binAddMediaButton_clicked();
    void on_binDeleteMediaButton_clicked();

    /* action bar events */
    void on_settingsAction_triggered();
    void on_lockSettingsAction_triggered();
    void on_menuVideoMode_triggered(QAction *action);

    /*parameters events*/
    void on_advancedSettingsButton_clicked();
    void on_advancedPictureSettingsButton_clicked();
    void on_lockButton_clicked();

    /* player controls */
    void on_playerPlayButton_clicked(bool checked);

    void on_playlistsTabWidget_currentChanged(int index);

    void on_subtitlesSyncSpinBox_valueChanged(double arg1);
    void on_gammaSpinBox_valueChanged(int arg1);
    void on_contrastSpinBox_valueChanged(int arg1);
    void on_brightnessSpinBox_valueChanged(int arg1);
    void on_saturationSpinBox_valueChanged(int arg1);
    void on_hueSpinBox_valueChanged(int arg1);
    void on_audioSyncDoubleSpinBox_valueChanged(double arg1);

    void on_testPatternAction_triggered();

    void on_saveAsAction_triggered();
    void on_openListingAction_triggered();

    void updateSettings();

    void createPlaylistTab();
    void on_playlistsTabWidget_tabCloseRequested(int index);
    void on_renamePlaylistAction_triggered();
    void on_removePlaylistItemAction_triggered();

    void on_audioTrackComboBox_currentIndexChanged(const QString &arg1);

    void on_videoTrackComboBox_currentIndexChanged(const QString &arg1);

    void on_subtitlesTrackComboBox_currentIndexChanged(const QString &arg1);

    int getTrackIndex(QList<int> list, int track);

protected:
    Playback* selectedPlayback() const;
    PlaylistTableView* currentPlaylistTableView() const;
    PlaylistModel* currentPlaylistModel() const;

private:
    Ui::MainWindow *ui;
    VideoWindow *_videoWindow;

    SettingsWindow *_settingsWindow;
    LockSettingsWindow *_lockSettingsWindow;
    AdvancedSettingsWindow *_advancedSettingsWindow;
    AdvancedPictureSettingsWindow *_advancedPictureSettingsWindow;
    StatusWidget *_statusWidget;

    QDataWidgetMapper *_mediaSettingsMapper;

    Application *_app;
    PlaylistPlayer *_playlistPlayer;

    MediaListModel *_mediaListModel;
    ScheduleListModel *_scheduleListModel;

    Locker *_locker;
};

#endif // MAINWINDOW_H
