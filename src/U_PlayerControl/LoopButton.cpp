#include <QIcon>
#include <QEvent>
#include <QGraphicsDropShadowEffect>

#include "LoopButton.h"
#include "PlaylistPlayer.h"

LoopButton::LoopButton(PlaylistPlayer* playlistPlayer, QWidget *parent) :
    QPushButton(parent)
{
    _playlistPlayer = playlistPlayer;

    setMinimumSize(QSize(50, 0));
    setCheckable(false);

    connect(this, SIGNAL(clicked()), this, SLOT(onClick()));

    QIcon icon;
    icon.addFile(QString::fromUtf8(":/icons/resources/glyphicons/bigLoop.png"), QSize(), QIcon::Normal, QIcon::Off);
    setIcon(icon);

    _state = NOLOOP;
    _playlistPlayer->setLoop(PlaylistPlayer::NOLOOP);
}

void LoopButton::onClick()
{
    if(_state == NOLOOP){
        _state = BIGLOOP;
        _playlistPlayer->setLoop(PlaylistPlayer::BIGLOOP);

        setStyleSheet("border: 1px solid rgb(100, 100, 100)");

        QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
        effect->setBlurRadius(8);
        effect->setOffset(0);
        effect->setColor(QColor(80, 80, 80));
        setGraphicsEffect(effect);
    }
    else if(_state == BIGLOOP){
        _state = SINGLELOOP;
        _playlistPlayer->setLoop(PlaylistPlayer::SINGLELOOP);

        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/resources/glyphicons/simpleLoop.png"), QSize(), QIcon::Normal, QIcon::Off);
        setIcon(icon);
    }
    else if(_state == SINGLELOOP){
        _state = NOLOOP;
        _playlistPlayer->setLoop(PlaylistPlayer::NOLOOP);

        setStyleSheet("border: 1px solid rgb(180, 180, 180)");

        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/resources/glyphicons/bigLoop.png"), QSize(), QIcon::Normal, QIcon::Off);
        setIcon(icon);

        // Remove the graphical effect
        setGraphicsEffect(0);
    }
}
