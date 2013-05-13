#include "automationwidget.h"
#include "ui_automationwidget.h"

AutomationWidget::AutomationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AutomationWidget)
{
    ui->setupUi(this);
}

AutomationWidget::~AutomationWidget()
{
    delete ui;
}
