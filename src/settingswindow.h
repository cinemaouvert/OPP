/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Cyril Naud <futuramath@gmail.com>
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
     */
    int getIndex(QString lang);
    /**
     * @brief Get the language associed to an index
     * @param index The index
     * @return The language associed to the index
     */
    QString getLang(int index);
    
private slots:
    /**
     * @brief Save modified settings and close the window
     * @param
     * @return
     */
    void on_buttonBox_accepted();

    /**
     * @brief Close the window
     * @param
     * @return
     */
    void on_buttonBox_rejected();

    /**
     * @brief Change the value of the VLC path field
     * @param
     * @return
     */
    void on_pushButton_VLCPath_clicked();

    /**
     * @brief Change the value of the test pattern path field
     * @param
     * @return
     */
    void on_pushButton_testPatternPath_clicked();

    /**
     * @brief Change the value of the intertitle path field
     * @param
     * @return
     */
    void on_pushButton_intertitlePath_clicked();

    /**
     * @brief Change the value of the movies path field
     * @param
     * @return
     */
    void on_pushButton_moviesPath_clicked();

private:
    /**
     * @brief ui The UI
     */
    Ui::SettingsWindow *ui;
};

#endif // SETTINGSWINDOW_H
