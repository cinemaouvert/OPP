#include "saturationwidget.h"
#include "ui_saturationwidget.h"

SaturationWidget::SaturationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SaturationWidget)
{
    ui->setupUi(this);
}

SaturationWidget::~SaturationWidget()
{
    delete ui;
}
