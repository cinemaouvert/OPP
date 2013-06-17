/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
 *          Baptiste Rozière <bapt.roziere@gmail.com>
 *          Hamza Haddar <ham.haddar@gmail.com>
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
class AboutDialog;
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
    /**
     * @brief Restore playlist tab
     * @param model
     */
    void restorePlaylistTab(PlaylistModel *model);
    /**
     * @brief Returns the playlists tab widget
     */
    QTabWidget * playlistTabWidget() const;

    /**
     * @brief Edit the name of the current playlist
     */
    void editPlaylistName();
    /**
     * @brief Delete the selected item on the playlist
     */
    void deletePlaylistItem();

private slots:
    /**
     * @brief Change the value of Ratio settings
     * @param index Index of the new Ratio
     */
    void on_ratioComboBox_currentIndexChanged(int index);

    /* bin events */
    /**
     * @brief Add a media in the bin
     */
    void on_binAddMediaButton_clicked();
    /**
     * @brief Remove a media from the bin
     */
    void on_binDeleteMediaButton_clicked();

    /**
     * @brief Update project summary
     */
    void updateProjectSummary();

    /* action bar events */
    /**
     * @brief Open the settings window
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_settingsAction_triggered();
    /**
     * @brief Open the lock settings window
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_lockSettingsAction_triggered();
    /**
     * @brief Change the video mode
     */
    void on_menuVideoMode_triggered(QAction *action);

    /*parameters events*/
    /**
     * @brief Open the advanced settings window
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_advancedSettingsButton_clicked();
    /**
     * @brief Open the advanced picture settings window
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_advancedPictureSettingsButton_clicked();

    /* player controls */
    /**
     * @brief Launch the playback of the playlist
     */
    void on_playerPlayButton_clicked(bool checked);

    /**
     * @brief Load the selected playlist
     */
    void on_playlistsTabWidget_currentChanged(int index);

    /**
     * @brief Change the value of subtitles sync setting
     * @param arg1 The new value for subtitles sync
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_subtitlesSyncSpinBox_valueChanged(double arg1);
    /**
     * @brief Change the value of gamma setting
     * @param arg1 The new value for gamma
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_gammaSpinBox_valueChanged(double arg1);
    /**
     * @brief Change the value of contrast setting
     * @param arg1 The new value for contrast
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_contrastSpinBox_valueChanged(double arg1);
    /**
     * @brief Change the value of brightness setting
     * @param arg1 The new value for brightness
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_brightnessSpinBox_valueChanged(double arg1);
    /**
     * @brief Change the value of saturation setting
     * @param arg1 The new value for saturation
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_saturationSpinBox_valueChanged(double arg1);
    /**
     * @brief Change the value of hue setting
     * @param arg1 The new value for hue
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_hueSpinBox_valueChanged(int arg1);
    /**
     * @brief Change the value of audio sync setting
     * @param arg1 The new value for audio sync
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_audioSyncDoubleSpinBox_valueChanged(double arg1);
    /**
     * @brief Change the value of audio gain setting
     * @param arg1 The new value for audio gain
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_audioGainDoubleSpinBox_valueChanged(double arg1);

    /**
     * @brief Play a test pattern
     */
    void on_testPatternAction_triggered();

    /**
     * @brief Save the current project
     */
    void on_saveAsAction_triggered();
    /**
     * @brief Open a saved project
     */
    void on_openListingAction_triggered();
    /**
     * @brief Create a new project
     */
    void on_newListingAction_triggered();

    /**
     * @brief Update settings value in UI
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void updateSettings();

    /**
     * @brief Create a new playlist and its tab
     */
    void createPlaylistTab();
    /**
     * @brief Delete a playlist and close its tab
     * @param index The index of the playlist to delete
     */
    void on_playlistsTabWidget_tabCloseRequested(int index);
    /**
     * @brief Open a window to rename the current playlist
     */
    void on_renamePlaylistAction_triggered();
    /**
     * @brief Delete the selected item
     */
    void on_removePlaylistItemAction_triggered();

    /**
     * @brief Change the value of the audio track setting
     * @param index Index of the new audio track
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_audioTrackComboBox_currentIndexChanged(int index);
    /**
     * @brief Change the value of the video track setting
     * @param index Index of the new video track
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_videoTrackComboBox_currentIndexChanged(int index);
    /**
     * @brief Change the value of the subtitles track setting
     * @param index Index of the new subtitles track
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_subtitlesTrackComboBox_currentIndexChanged(int index);

    /**
     * @brief Update the list of playlists
     */
    void updatePlaylistListCombox();

    /**
     * @brief Create a new schedule
     */
    void on_scheduleAddButton_clicked();
    /**
     * @brief Delete the selected schedule
     */
    void on_scheduleDeleteButton_clicked();
    /**
     * @brief Delay the schedules
     */
    void on_scheduleDelayButton_clicked();
    /**
     * @brief Enable or disable automation
     * @param checked True to enable automation, false to disable
     */
    void on_scheduleToggleEnabledButton_toggled(bool checked);

    /**
     * @brief Move the selected playback up
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_playlistUpButton_clicked();
    /**
     * @brief Move the selected playback down
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_playlistDownButton_clicked();

    /**
     * @brief Change the notes
     */
    void on_notesEdit_textChanged();

    /**
     * @brief Create a new playlist
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_addPlaylistButton_clicked();

    /**
     * @brief Edit playlist name
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_editNamePlaylistButton_clicked();

    /**
     * @brief Delete selected item
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_deletePlaylistItemButton_clicked();
    /**
     * @brief Open About window
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_aboutAction_triggered();

protected:
    /**
     * @brief Returns the selected playback
     * @return The selected playback
     */
    Playback* selectedPlayback() const;
    /**
     * @brief Returns the current playlist table view
     * @return The current playlist table view
     */
    PlaylistTableView* currentPlaylistTableView() const;
    /**
     * @brief Returns the current playlist model
     * @return The current playlist model
     */
    PlaylistModel* currentPlaylistModel() const;
    /**
     * @brief Returns the playlist at the position index
     * @return The playlist at the position index
     */
    Playlist* playlistAt(int index) const;

private:
    /**
     * @brief ui The UI
     */
    Ui::MainWindow *ui;
    /**
     * @brief _videoWindow The video window
     */
    VideoWindow *_videoWindow;

    /**
     * @brief _settingsWindow The settings window
     */
    SettingsWindow *_settingsWindow;
    /**
     * @brief _lockSettingsWindow The lock settings window
     */
    LockSettingsWindow *_lockSettingsWindow;
    /**
     * @brief _advancedSettingsWindow The advanced settings window
     */
    AdvancedSettingsWindow *_advancedSettingsWindow;
    /**
     * @brief _advancedPictureSettingsWindow The advanced picture settings window
     */
    AdvancedPictureSettingsWindow *_advancedPictureSettingsWindow;
    /**
     * @brief _about The About window
     */
    AboutDialog *_aboutdialog;
    /**
     * @brief _statusWidget The status widget
     */
    StatusWidget *_statusWidget;

    /**
     * @brief _app
     */
    Application *_app;
    /**
     * @brief _playlistPlayer
     */
    PlaylistPlayer *_playlistPlayer;

    /**
     * @brief _mediaListModel The media list model
     */
    MediaListModel *_mediaListModel;
    /**
     * @brief _scheduleListModel The schedule list model
     */
    ScheduleListModel *_scheduleListModel;

    /**
     * @brief _locker The locker
     */
    Locker *_locker;

    /**
     * @brief _dataStorage
     */
    DataStorage* _dataStorage;
};

#endif // MAINWINDOW_H
