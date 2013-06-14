#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDataWidgetMapper>
#include <QLabel>
#include<QModelIndexList>
#include<QTime>

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
class Playlist;
class Locker;
class DataStorage;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // FIX : ref 0000001
    void restorePlaylistTab(PlaylistModel *model);

private slots:
    void on_ratioComboBox_currentIndexChanged(int index);

    /* bin events */
    void on_binAddMediaButton_clicked();
    void on_binDeleteMediaButton_clicked();

    /*projecttabwidget */
    void setSummary(int count);
    void setDetails(int count);
    //void setMediaCount();

    /* action bar events */
    void on_settingsAction_triggered();
    void on_lockSettingsAction_triggered();
    void on_menuVideoMode_triggered(QAction *action);

    /*parameters events*/
    void on_advancedSettingsButton_clicked();
    void on_advancedPictureSettingsButton_clicked();

    /* player controls */
    void on_playerPlayButton_clicked(bool checked);

    void on_playlistsTabWidget_currentChanged(int index);

    void on_subtitlesSyncSpinBox_valueChanged(double arg1);
    void on_gammaSpinBox_valueChanged(double arg1);
    void on_contrastSpinBox_valueChanged(double arg1);
    void on_brightnessSpinBox_valueChanged(double arg1);
    void on_saturationSpinBox_valueChanged(double arg1);
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

    void on_audioTrackComboBox_currentIndexChanged(int index);
    void on_videoTrackComboBox_currentIndexChanged(int index);
    void on_subtitlesTrackComboBox_currentIndexChanged(int index);

    void updatePlaylistListCombox();

    int getTrackIndex(QList<int> list, int track);

    void on_scheduleAddButton_clicked();
    void on_scheduleDeleteButton_clicked();
    void on_scheduleDelayButton_clicked();
    void on_scheduleToggleEnabledButton_toggled(bool checked);
    void on_audioGainDoubleSpinBox_valueChanged(double arg1);

    void on_playlistUpButton_clicked();
    void on_playlistDownButton_clicked();

    void on_notesEdit_textChanged();

protected:
    Playback* selectedPlayback() const;
    PlaylistTableView* currentPlaylistTableView() const;
    PlaylistModel* currentPlaylistModel() const;
    Playlist* playlistAt(int index) const;

private:
    Ui::MainWindow *ui;
    VideoWindow *_videoWindow;

    SettingsWindow *_settingsWindow;
    LockSettingsWindow *_lockSettingsWindow;
    AdvancedSettingsWindow *_advancedSettingsWindow;
    AdvancedPictureSettingsWindow *_advancedPictureSettingsWindow;
    StatusWidget *_statusWidget;

    Application *_app;
    PlaylistPlayer *_playlistPlayer;

    MediaListModel *_mediaListModel;
    ScheduleListModel *_scheduleListModel;

    Locker *_locker;

    DataStorage* _dataStorage;
};

#endif // MAINWINDOW_H
