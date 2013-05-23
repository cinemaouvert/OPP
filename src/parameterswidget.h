#ifndef PARAMETERSWIDGET_H
#define PARAMETERSWIDGET_H

#include <QWidget>
#include "advancedsettingswindow.h"
#include "advancedpicturesettingswindow.h"
#include "locksettingswindow.h"

namespace Ui {
class ParametersWidget;
}

class ParametersWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ParametersWidget(QWidget *parent = 0);
    ~ParametersWidget();

private slots:
    void on_pushButton_advancedSettings_clicked();
    void on_pushButton_advancedPictureSettings_clicked();
    void on_pushButton_lock_clicked();
    
private:
    Ui::ParametersWidget *ui;
    AdvancedSettingsWindow *advancedSettingsWindow;
    AdvancedPictureSettingsWindow *advancedPictureSettingsWindow;
    LockSettingsWindow *lockSettingsWindow;
};

#endif // PARAMETERSWIDGET_H
