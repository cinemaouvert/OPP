#include "playertimelinewidget.h"
#include "ui_playertimelinewidget.h"

PlayerTimelineWidget::PlayerTimelineWidget(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::PlayerTimelineWidget)
{
   ui->setupUi(this);
   ui->currentPlaybackVideo->setStyleSheet("background-color: black");
}

PlayerTimelineWidget::~PlayerTimelineWidget()
{
   delete ui;
}
