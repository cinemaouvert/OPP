#include "resumedetailswidget.h"
#include "ui_resumedetailswidget.h"

ResumeDetailsWidget::ResumeDetailsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResumeDetailsWidget)
{
    ui->setupUi(this);
}

ResumeDetailsWidget::~ResumeDetailsWidget()
{
    delete ui;
}
