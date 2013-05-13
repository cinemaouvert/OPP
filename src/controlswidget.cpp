#include "controlswidget.h"
#include "ui_controlswidget.h"

ControlsWidget::ControlsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlsWidget)
{
    ui->setupUi(this);
}

ControlsWidget::~ControlsWidget()
{
    delete ui;
}
