#include "playertimelinewidget.h"
#include "ui_playertimelinewidget.h"

#include "media.h"
#include "mediaplayer.h"

#include <QDebug>

PlayerTimelineWidget::PlayerTimelineWidget(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::PlayerTimelineWidget),
   _mediaPlayer(NULL),
   _nextMedia(NULL),
   _futurMedia(NULL)
{
   ui->setupUi(this);
}

PlayerTimelineWidget::~PlayerTimelineWidget()
{
   delete ui;
}

void PlayerTimelineWidget::setMediaPlayer(MediaPlayer *mediaPlayer)
{
    ui->seekWidget->setMediaPlayer(mediaPlayer);
}

void PlayerTimelineWidget::setNextMedia(Media *media)
{
    _nextMedia = media;
}

void PlayerTimelineWidget::setFutureMedia(Media *media)
{
    _futurMedia = media;
}
