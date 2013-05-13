#include "binwidget.h"
#include "ui_binwidget.h"

BinWidget::BinWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BinWidget)
{
    ui->setupUi(this);
}

BinWidget::~BinWidget()
{
    delete ui;
}
