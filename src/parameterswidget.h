#ifndef PARAMETERSWIDGET_H
#define PARAMETERSWIDGET_H

#include <QWidget>
#include "advancedsettingswindow.h"

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
    
private:
    Ui::ParametersWidget *ui;
    AdvancedSettingsWindow *advancedSettingsWindow;
};

#endif // PARAMETERSWIDGET_H
