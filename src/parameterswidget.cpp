#include "parameterswidget.h"
#include "ui_parameterswidget.h"
#include "advancedsettingswindow.h"
#include "advancedpicturesettingswindow.h"

ParametersWidget::ParametersWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParametersWidget)
{
    ui->setupUi(this);
}

ParametersWidget::~ParametersWidget()
{
    delete ui;
}

void ParametersWidget::on_pushButton_advancedSettings_clicked()
{
    advancedSettingsWindow = new AdvancedSettingsWindow(this);
    advancedSettingsWindow->show();
    advancedSettingsWindow->raise();
    advancedSettingsWindow->activateWindow();
}

void ParametersWidget::on_pushButton_advancedPictureSettings_clicked()
{
    advancedPictureSettingsWindow = new AdvancedPictureSettingsWindow(this);
    advancedPictureSettingsWindow->show();
    advancedPictureSettingsWindow->raise();
    advancedPictureSettingsWindow->activateWindow();
}
