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

#ifndef ADVANCEDSETTINGSWINDOW_H
#define ADVANCEDSETTINGSWINDOW_H

#include <QDialog>

namespace Ui {
class AdvancedSettingsWindow;
}
class Playback;
class AdvancedSettingsWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit AdvancedSettingsWindow(QWidget *parent = 0);
    ~AdvancedSettingsWindow();
    void setPlayback(Playback* playback);
    void updateLength();
    
private slots:
    void on_buttonBox_OKCancel_accepted();

    void on_buttonBox_OKCancel_rejected();

    void on_timeEdit_inMark_timeChanged(const QTime &date);

    void on_timeEdit_outMark_timeChanged(const QTime &date);

private:
    Ui::AdvancedSettingsWindow *ui;
    Playback* _playback;
};

#endif // ADVANCEDSETTINGSWINDOW_H
