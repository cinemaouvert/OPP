/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Florian Mhun <florian.mhun@gmail.com>
 *          Cyril Naud <futuramath@gmail.com>
 *          Baptiste Rozière <bapt.roziere@gmail.com>
 *          Hamza Haddar <ham.haddar@gmail.com>
 *          Geoffrey Bergé <geoffrey.berge@live.fr>
 *          Thomas Berthomé <thoberthome@laposte.net>
 *          Thibaud Lamarche <lamarchethibaud@hotmail.com>
 *          Denis Saunier <saunier.denis.86@gmail.com>
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
#include <vlc/vlc.h>

#include "plugins.h"

#include "loggersingleton.h"
#include "videowindow.h"

#include "PlaylistHandlerWidget.h"

#include "PlayerControlWidget.h"

namespace Ui {
class MainWindow;
}

class VideoWindow;
class SettingsWindow;
class LockSettingsWindow;
class AdvancedSettings;
class AdvancedPictureSettingsWindow;
class AboutDialog;
class PlaylistTableView;
class PlaylistHandlerWidget;
class StatusWidget;
class PlaylistTabWidget;

class PlaylistModel;
class MediaListModel;
class ScheduleListModel;

class VLCApplication;
class PlaylistPlayer;
class PlayerControlWidget;
class Playback;
class Playlist;
class Locker;
class DataStorage;
class ExportPDF;
class MediaPlayer;
class Media;


class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /**
     * @brief return the playlist player
     */
    inline PlaylistPlayer* playlistPlayer() const { return _playlistPlayer; }

    /**
     * @brief Getlocker
     *
     * @author Geoffrey Bergé <geoffrey.berge@live.fr>
     */
    Locker* locker() const { return _locker; }

    /**
     * @brief filename
     * @return
     */
    inline QString filename() { return _fileName; }

    /**
     * @brief playlistTabWidget
     * @return
     */
    PlaylistTabWidget* playlistTabWidget() const { return _playlistTabWidget; }

    /**
     * @brief scheduleListModel
     * @return
     */
    inline ScheduleListModel* scheduleListModel(){ return _scheduleListModel; }

    /**
     * @brief mediaListModel
     * @return
     */
    inline MediaListModel* mediaListModel(){ return _mediaListModel; }

    /**
     * @brief playerControlWidget
     * @return
     */
    PlayerControlWidget* playerControlWidget(){ return _playerControlWidget; }

    QLabel* screenBefore() const;

    QLabel* screenBack() const;

    /**
     * @brief request save if needed
     *
     * @author Geoffrey Bergé <geoffrey.berge@live.fr>
     */
    int verifSave();

    /**
     * @brief open the listing from the fileName
     *
     * @author Geoffrey Bergé <geoffrey.berge@live.fr>
     */
    void openListing(QString fileName);

    void takeScreenshot(QStringList fileNames);

    void takeScreenshot(QString fileName);

    /**
     * @brief Get _playlistPlayer
     *
     * @author Thomas Berthomé <thoberthome@laposte.net>
     */
    inline PlaylistPlayer* getPlaylistPlayer() const {return _playlistPlayer;}

    /**
      * @brief Method used to set the selected media name
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.com>
      */
    void setSelectedMediaNameByIndex(int idx);

    /**
      * @brief Method used to set the previous and following selected medium time
      *
      * @author Thomas Berthome <thoberthome@laposte.net>
      */
    void setSelectedMediaTimeByIndex(int idx);

    /**
     * @brief addMedia
     * @param location
     */
    int addMedia(QString location);

	/**
      * @brief Create schedule in html
      *
      * @author Denis Sauneir <saunier.denis.86@gmail.com>
      */
    QString scheduleToHml();

    /**
      * @brief Create schedule in html for pdf
      *
      * @author Denis Sauneir <saunier.denis.86@gmail.com>
      */
    QString scheduleToHmlForPDF();

    VideoWindow* videoWindow() const { return _videoWindow; }

    /**
      * @brief Redirect qmessages into a file and the log area
      *
      * @author Thomas Berthome <thoberthome@laposte.net>
      */
    #if (QT_VERSION >= 0x050000) // Qt version 5 and above
        static void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString& msg);
    #else // until version 5
        static void myMessageHandler(QtMsgType type, const char* msg);
    #endif

public slots:

    /**
     * @brief Used to relaunch the video window if it was closed
     *
     * @author Lamarche Thibaud <lamarchethibaud@hotmail.fr>
     */
    void needVideoWindow();

    /**
      *@brief Update the remaining time
      */
    void stop();

    /**
     * @brief Save the current project
     */
    void on_saveAction_triggered();

     /**
     * @brief Open the advanced settings window
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void openAdvancedSettingsWindow();

    /**
     * @brief Open the advanced picture settings window
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void openAdvancedPictureSettings();

	/**
     * @brief Set a new screenshot in the screenBack
     *
     * @author Thomas Berthomé <thoberthome@laposte.net>
     */
    void setScreenshot(QString url);

    /**
      *@brief Method used to set the previous and following selected medium back
      *
      * @author Thomas Berthome <thoberthome@laposte.net>
      */
    void setScreensBack(QString url);

    /**
     * @brief Used to update the current screenshot
     *
     * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
     */
    void updateCurrentScreenshot();

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
     * @brief Update project summary
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void updateProjectSummary();

    /**
     * @brief Update settings value in UI
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void updateSettings();

    /**
     * @brief Update Details
     *
     * @author Geoffrey Bergé <geoffrey.berge@live.fr>
     */
    void updateDetails();

    /**
     * @brief switchVideoMode
     */
    void switchVideoMode();

    /**
     * @brief Update the list of playlists
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void updatePlaylistListCombox();

private slots:

    /**
     * @brief Show timeout before the end of the current playlist
     *
     * @author Lamarche Thibaud <lamarchethibaud@hotmail.fr>
     */
    void showTimeOut();

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
     * @brief Add a media to the project
     */
    void on_addMediaAction_triggered();

    /**
     * @brief Save the current project
     */
    void on_saveAsAction_triggered();

    /**
     * @brief Quit the software
     */
    void on_quitAction_triggered();

    /**
     * @brief Action to pen a saved project
     */
    void on_openListingAction_triggered();

    /**
     * @brief Create a new project
     */
    void on_newListingAction_triggered();

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
     * @brief Change the notes
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void on_notesEdit_textChanged();

    /**
     * @brief Open About window
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_aboutAction_triggered();

    /**
     * @brief Update software OPP
     *
     * @author Denis Saunier <saunier.denis.86@gmail.com>
     */
    void on_updateAction_triggered();

    /**
     * @brief Get Locked Widget
     *
     * @author Thomas Berthome <thoberthome@laposte.net>
     */
    QList<QWidget*> getLockedWidget();

    /**
      * @brief slot for the plugin action
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
      */
    void ocpmSecondaryAction();

    /**
      * @brief slot for the launch of the mire
      * @param fileName The mire filename
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
      */
    void playMire(QString fileName);

    /**
     * @brief change subtitles encode
     * @param index Index of the subtitle encode
     *
     * @author Denis Saunier <saunier.denis.86@gmail.com>
     */
    void on_subtitlesEncodecomboBox_currentIndexChanged(int index);

    /**
     * @brief viex export pdf
     *
     * @author Denis Saunier <saunier.denis.86@gmail.com>
     */
    void on_viewExportPDFButton_clicked();

    /**
      * @brief slot to close the mire player
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
      */
    void closeMirePlayer();

    /**
      * @brief slot to close the window of the mire player
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
      */
    void closeWindowTestPattern();

    void on_actionTest_patterns_triggered();

    void on_actionScreenshots_triggered();

    void on_actionPlugins_triggered();

    void on_helpAction_triggered();

    void on_actionLog_triggered();

protected:
    /**
     * @brief Returns the selected playback
     * @return The selected playback
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    Playback* selectedPlayback() const;

private:

    /**
     * @brief Handle the close event.
     */
    void closeEvent (QCloseEvent *event);

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
    AdvancedSettings *_advancedSettingsWindow;

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
    VLCApplication *_app;

    /**
     * @brief _playlistPlayer
     */
    PlaylistPlayer *_playlistPlayer;

    /**
     * @brief _playerControlWidget
     */
    PlayerControlWidget* _playerControlWidget;

    /**
     * @brief _playlistHandlerWidget
     */
    PlaylistHandlerWidget* _playlistHandlerWidget;

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

    /**
      * @brief store the plugin
      */
    OCPM* _ocpmPlugin;

    /**
      * @brief show pdf of schedule
      */
    ExportPDF* _exportPDF;

    /**
     * @brief logger
     */
    LoggerSingleton *_logger;

    /**
     * @brief vlc instance for the test pattern
     */
    libvlc_instance_t *_vlcMire;

    /**
     * @brief Media player for the test pattern
     */
    MediaPlayer *_mpMire;

    /**
     * @brief media for the test pattern
     */
    Media *_mireMire;

    /**
     * @brief Playback for the test pattern
     */
    Playback *_pbMire;

    /**
     * @brief VideoWindow for the test pattern
     */
    VideoWindow *_vWMire;

    /**
     * @brief ui The timer for automation
     */
    QTimer *_timerOut;

    PlaylistTabWidget* _playlistTabWidget;

    QShortcut* _f1_shortcut;

    /**
     * @brief open dir and create if not exits
     */
    void openDir(QString name);
};

#endif // MAINWINDOW_H
