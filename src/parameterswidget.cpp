#include "parameterswidget.h"
#include "ui_parameterswidget.h"

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
