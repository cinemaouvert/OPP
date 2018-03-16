/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Cyril Naud <futuramath@gmail.com>
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

#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QShortcut>
#include "mainwindow.h"


namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();

    /**
     * @brief initializes shortcut settings comboboxes
     */
    void initShorcuctsSettings();

    /**
     * @brief resets shortcut settings to default
     */
    void resetShorcuctsSettingsDefault();

    /**
     * @brief Get the index of a language
     * @param lang The language
     * @return The index of the language
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    int getIndex(QString lang);
    /**
     * @brief Get the language associed to an index
     * @param index The index
     * @return The language associed to the index
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    QString getLang(int index);
    
    void applyShortcuts();
    void initShortcuts();
    void loadShortcuts();
    void connectShortcuts();
    void disconnectShortcuts();
private slots:
    /**
     * @brief Save modified settings and close the window
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_buttonBox_accepted();

    /**
     * @brief Close the window
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_buttonBox_rejected();

    /**
     * @brief Change the value of the movies path field
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_pushButton_moviesPath_clicked();

    /**
     * @brief Activate the group box for the screen position and show an information message
     *
     * @author Thomas Berthome <thoberthome@laposte.net>
     */
    void on_radioButton_Streaming_clicked();

    /**
     * @brief Desactivate the group box for the screen position and show an information message
     *
     * @author Thomas Berthome <thoberthome@laposte.net>
     */
    void on_radioButton_Pictures_clicked();

    /**
     * @brief Desactivate the group box for the screen position and show an information message
     *
     * @author Thomas Berthome <thoberthome@laposte.net>
     */
    void on_radioButton_None_clicked();

    /**
     * @brief Save modified settings, close the window and restart the software
     *
     * @author Geoffrey Bergé <geoffrey.berge@live.fr>
     */
    void on_restart_clicked();

    /**
     * @brief on_openLastUsedListing_clicked
     */
    void on_openLastUsedListing_clicked();

    /**
     * @brief sets combobox to default value
     */
    void on_playPauseDefaultButton_clicked();

    /**
     * @brief sets combobox to default value
     */
    void on_stopDefaultButton_clicked();

    /**
     * @brief sets combobox to default value
     */
    void on_previousDefaultButton_clicked();

    /**
     * @brief sets combobox to default value
     */
    void on_nextDefaultButton_clicked();

    /**
     * @brief sets combobox to default value
     */
    void on_rewindDefaultButton_clicked();

    /**
     * @brief sets combobox to default value
     */
    void on_forwardDefaultButton_clicked();

    /**
     * @brief sets combobox to default value
     */
    void on_lockDefaultButton_clicked();

    /**
     * @brief sets combobox to default value
     */
    void on_loopDefaultButton_clicked();

    /**
     * @brief sets combobox to default value
     */
    void on_switchVideoModesDefaultButton_clicked();

    /**
     * @brief sets combobox to default value
     */
    void on_defaultShortcutsButton_clicked();

    void on_playPause_changed();
    void on_stop_changed();
    void on_previous_changed();
    void on_next_changed();
    void on_rewind_changed();
    void on_forward_changed();
    void on_loop_changed();
    void on_lock_changed();
    void on_switchVideoModes_changed();

private:
    /**
     * @brief ui The UI
     */
    Ui::SettingsWindow *ui;

    QShortcut* playPause_shortcut;
    QShortcut* previous_shortcut;
    QShortcut* next_shortcut;
    QShortcut* rewind_shortcut;
    QShortcut* forward_shortcut;
    QShortcut* stop_shortcut;
    QShortcut* loop_shortcut;
    QShortcut* lock_shortcut;
    QShortcut* switchVideoModes_shortcut;
    QStringList listOfShortcuts;
    MainWindow* _mw;

    /**
     * @brief set the value of the video return mode
     *
     * @author  Thibaud Lamarche <lamarchethibaud@hotmail.fr>
     */
    void setVideoReturnMode();

    /**
     * @brief set the value of the stteings video return mode
     *
     * @author  Thibaud Lamarche <lamarchethibaud@hotmail.fr>
     */
    void setSettingsVideoReturnMode();

    /**
     * @brief init the buttons
     *
     * @author  Thibaud Lamarche <lamarchethibaud@hotmail.fr>
     */
    void init();

    /**
     * @brief accept the settings
     *
     * @author  Thibaud Lamarche <lamarchethibaud@hotmail.fr>
     */
    void accept();


};

#endif // SETTINGSWINDOW_H
