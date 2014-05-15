/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Cyril Naud <futuramath@gmail.com>
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

#include "mediaplayer.h"

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
     * @brief Change the value of the VLC path field
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_pushButton_VLCPath_clicked();

    /**
     * @brief Change the value of the test pattern path field
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_pushButton_testPatternPath_clicked();

    /**
     * @brief Change the value of the intertitle path field
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_pushButton_intertitlePath_clicked();

    /**
     * @brief Change the value of the movies path field
     *
     * @author Cyril Naud <futuramath@gmail.com>
     */
    void on_pushButton_moviesPath_clicked();

    void on_radioButton_Streaming_clicked();

    void on_radioButton_Pictures_clicked();

    void on_radioButton_None_clicked();

    void on_restart_clicked();

private:
    /**
     * @brief ui The UI
     */
    Ui::SettingsWindow *ui;

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
