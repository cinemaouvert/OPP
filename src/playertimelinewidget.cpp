#include "playertimelinewidget.h"
#include "ui_playertimelinewidget.h"

PlayerTimelineWidget::PlayerTimelineWidget(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::PlayerTimelineWidget)
{
   ui->setupUi(this);
}

PlayerTimelineWidget::~PlayerTimelineWidget()
{
   delete ui;
}
