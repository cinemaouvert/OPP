/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Thibaud Lamarche <lamarchethibaud@hotmail.fr>
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

#ifndef PLUGINS_H
#define PLUGINS_H

#include <QtPlugin>
#include <QDialog>

/************** OCPM PLUG-IN****************/

class MainWindow;


/**
  * @brief Class used for OCPM plugin
  *
  * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
  */
class OCPM : public QDialog {
public:
    virtual ~OCPM() { }

    virtual QString getName() = 0;

    /**
      * @brief Set the filename
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
      */
    virtual void setFilename(QString * filename)=0;

    /**
      * @brief Used to add an action for the plugin
      *
      * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
      */
    virtual void secondaryAction() =0;


public slots:

    void launch();


protected:
    QString* _filename;
};
Q_DECLARE_INTERFACE(OCPM, "opp.OCPM")

/************** GENERIC PLUG-IN ****************/

/**
  * @brief Class used for Generic plugin
  *
  * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
  */
class OPP_GENERIC_PLUGIN{
    public:
        virtual ~OPP_GENERIC_PLUGIN() { }
        /**
          * @brief Return the name of the plugin
          *
          * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
          */
        virtual QString getName() = 0;

        /**
          * @brief Method used to set the pointer to the mainWindow (need to be launched by the plugin loader)
          *
          * @author Thibaud Lamarche <lamarchethibaud@hotmail.fr>
          */
        virtual void setMainWindow(MainWindow *mw) =0;

    protected:
        MainWindow *_mainWindow;
};
Q_DECLARE_INTERFACE(OPP_GENERIC_PLUGIN, "opp.OPP_GENERIC_PLUGIN")


#endif // PLUGINS_H
