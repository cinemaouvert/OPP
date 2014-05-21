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
#include <QModelIndexList>
#include <QTime>

#include "videowindow.h"

namespace Ui {
class MainWindow;
}

class VideoWindow;
class SettingsWindow;
class LockSettingsWindow;
class OCPM_Plugin;
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
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void restorePlaylistTab(PlaylistModel *model);

    /**
     * @brief Returns the playlists tab widget
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
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

    /**
     * @brief Getlocker
     *
     * @author Geoffrey Bergé <geoffrey.berge@live.fr>
     */
    Locker* getLocker();

    /**
     * @brief request save if needed
     *
     * @author Geoffrey Bergé <geoffrey.berge@live.fr>
     */
    void verifSave();

    /**
     * @brief GetFilename
     *
     * @author Geoffrey Bergé <geoffrey.berge@live.fr>
     */
    QString getFilename();

    /**
     * @brief open the listing from the fileName
     *
     * @author Geoffrey Bergé <geoffrey.berge@live.fr>
     */
    void openFile(QString fileName);

    /**
     * @brief Get _playlistPlayer
     *
     * @author Thomas Berthomé <thoberthome@laposte.net>
     */
    inline PlaylistPlayer* getPlaylistPlayer() const {return _playlistPlayer;}

    /**
      *@brief Method used to set the selected media name
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.com>
      */
    void setSelectedMediaNameByIndex(int idx);

public slots:
    /**
      *@brief Method to stop the player
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.com>
      */
    void stop();

    /**
     * @brief Update settings value in UI
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void updateSettings();

    /**
     * @brief Save the current project
     */
    void on_saveAction_triggered();

     /**
     * @brief Open the advanced settings window
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_advancedSettingsButton_clicked();

	/**
     * @brief Set a new screenshot in the screenBack
     *
     * @author Thomas Berthomé <thoberthome@laposte.net>
     */
    void setScreenshot(QString url);
private slots:

    /**
     * @brief Change the value of Ratio settings
     * @param index Index of the new Ratio
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void on_ratioComboBox_currentIndexChanged(int index);

    /**
     * @brief Add a media in the bin
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void on_binAddMediaButton_clicked();

    /**
     * @brief Remove a media from the bin
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void on_binDeleteMediaButton_clicked();

    /**
     * @brief Update project summary
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void updateProjectSummary();

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
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void on_menuVideoMode_triggered(QAction *action);

    /**
     * @brief Disable the small sample of video
     *
     * @author Thomas Berthomé <thoberthome@laposte.net>
     */
    void on_disableButton_clicked();

    /**
     * @brief Open the advanced picture settings window
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_advancedPictureSettingsButton_clicked();

    /**
     * @brief Launch the playback of the playlist
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void on_playerPlayButton_clicked(bool checked);

    /**
     * @brief Load the selected playlist
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
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
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void on_testPatternAction_triggered();

    /**
     * @brief Save the current project
     */
    void on_saveAsAction_triggered();

    /**
     * @brief Quit the software
     */
    void on_quitAction_triggered();


    /**
     * @brief Open a saved project
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void on_openListingAction_triggered();

    /**
     * @brief Create a new project
     *
     * @author Florian Mhun <florian.mhun@gmail.com>z
     */
    void on_newListingAction_triggered();

    /**
     * @brief Create a new playlist and its tab with a default name
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void createPlaylistTab();

    /**
     * @brief Create a new playlist and its tab with a name given by the user
     *
     * @author Thomas Berthome <thoberthome@laposte.net>
     */
    void createPlaylistTab(QString name);

    /**
     * @brief Delete a playlist and close its tab
     * @param index The index of the playlist to delete
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void on_playlistsTabWidget_tabCloseRequested(int index);

    /**
     * @brief Open a window to rename the current playlist
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void on_renamePlaylistAction_triggered();

    /**
     * @brief Delete the selected item
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void on_removePlaylistItemAction_triggered();

    /**
     * @brief Change the value of the audio track setting
     * @param index Index of the new audio track
     *
     * @author Cyril Naud <futuramath@gmail.com>
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void on_audioTrackComboBox_currentIndexChanged(int index);

    /**
     * @brief Change the value of the video track setting
     * @param index Index of the new video track
     *
     * @author Cyril Naud <futuramath@gmail.com>
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void on_videoTrackComboBox_currentIndexChanged(int index);

    /**
     * @brief Change the value of the subtitles track setting
     * @param index Index of the new subtitles track
     *
     * @author Cyril Naud <futuramath@gmail.com>
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void on_subtitlesTrackComboBox_currentIndexChanged(int index);

    /**
     * @brief Update the list of playlists
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void updatePlaylistListCombox();

    /**
     * @brief Create a new schedule
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void on_scheduleAddButton_clicked();

    /**
     * @brief Delete the selected schedule
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void on_scheduleDeleteButton_clicked();

    /**
     * @brief Delay the schedules
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void on_scheduleDelayButton_clicked();

    /**
     * @brief Enable or disable automation
     * @param checked True to enable automation, false to disable
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
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
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
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

    /**
     * @brief Get Locked Widget
     *
     * @author Thomas Berthome <thoberthome@laposte.net>
     */
    QList<QWidget*> getLockedWidget();



protected:
    /**
     * @brief Returns the selected playback
     * @return The selected playback
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    Playback* selectedPlayback() const;

    /**
     * @brief Returns the current playlist table view
     * @return The current playlist table view
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    PlaylistTableView* currentPlaylistTableView() const;

    /**
     * @brief Returns the current playlist model
     * @return The current playlist model
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    PlaylistModel* currentPlaylistModel() const;

    /**
     * @brief Returns the playlist at the position index
     * @return The playlist at the position index
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    Playlist* playlistAt(int index) const;

private:

    /**
      * @brief Loads the plugins from the folder plugins
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
      */
    void loadPlugins();

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
    OCPM_Plugin *_advancedSettingsWindow;
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

    /**
     * @brief used to disconnect signals
     */
    int _lastSelectedTab;

    /**
     * @brief _fileName
     */
    QString _fileName;

    /**
      * @brief store the selected projection mode
      */
    VideoWindow::DisplayMode _projectionMode;

    /**
      * @brief store the selected Media Name
      */
    QString* _selectedMediaName;
};

#endif // MAINWINDOW_H
