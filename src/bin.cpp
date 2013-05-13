#include "bin.h"
#include "ui_bin.h"

Bin::Bin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Bin)
{
    ui->setupUi(this);
}

Bin::~Bin()
{
    delete ui;
}
