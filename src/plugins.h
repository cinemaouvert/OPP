#ifndef PLUGINS_H
#define PLUGINS_H

#include <QtPlugin>
#include <QDialog>

class OCPM : public QDialog {
    public:
        virtual ~OCPM() { }

        virtual QString getName() = 0;

    public slots:
        virtual void launch(QString filename = "") = 0;

    protected:
        QString filename;
};
Q_DECLARE_INTERFACE(OCPM, "opp.OCPM")

#endif // PLUGINS_H
