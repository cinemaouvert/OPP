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
        virtual void setFilename(QString * filename)=0;

        /**
          *
          * @brief Used to ad an action for the plugin
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
