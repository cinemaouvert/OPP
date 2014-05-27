#ifndef PLUGINS_H
#define PLUGINS_H

#include <QtPlugin>
#include <QDialog>

class OCPM : public QDialog {
    public:
        virtual ~OCPM() { }

        virtual QString getName() = 0;
        virtual void setFilename(QString * filename)=0;

        /**
          *
          * @brief Used to ad an action for the plugin
          *
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

#endif // PLUGINS_H
