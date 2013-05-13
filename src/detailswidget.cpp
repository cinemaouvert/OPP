#include "detailswidget.h"
#include "ui_detailswidget.h"

DetailsWidget::DetailsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DetailsWidget)
{
    ui->setupUi(this);
}

DetailsWidget::~DetailsWidget()
{
    delete ui;
}
