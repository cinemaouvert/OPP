#include "ForwardButton.h"
#include "MediaPlayer.h"

ForwardButton::ForwardButton(PlaylistPlayer* playlistPlayer, QWidget *parent) :
    QPushButton(parent)
{
    _playlistPlayer = playlistPlayer;
    _locked = false;
    _timer = new QTimer(this);

    connect(_timer, SIGNAL(timeout()), this, SLOT(forward()));

    setCheckable(false);
    setChecked(false);

    setText("");

    setMinimumSize(QSize(50, 0));

    /** set the icon */
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_176_forward.png"), QSize(), QIcon::Normal, QIcon::Off);

    setIcon(icon);
}

ForwardButton::~ForwardButton()
{
    delete _timer;
}

void ForwardButton::mousePressEvent(QMouseEvent *e)
{
    e->ignore();

    setStyleSheet(QString(
        #if defined(Q_OS_MAC)
        "   margin: 0px 10px 0px 0px;"
        #else
        "   margin: 0px 0px 0px 0px;"
        #endif
        "   padding: 5px 0px 5px 0px;"
        "   border-radius: 0px;"
        "   border: 1px solid rgb(153, 153, 153);"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 rgb(220,220,220), stop:1 rgb(203,203,203));"
    ));

    _locked = true;

    forward();

    _timer->start(200);
}

void ForwardButton::mouseReleaseEvent(QMouseEvent* e)
{
    e->ignore();

    setStyleSheet(QString(
        "QPushButton{"
        #if defined(Q_OS_MAC)
        "   margin: 0px 10px 0px 0px;"
        #else
        "   margin: 0px 0px 0px 0px;"
        #endif
        "   padding: 5px 0px 5px 0px;"
        "   border-radius: 0px;"
        "   border: 1px solid rgb(190, 190, 190);"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 rgb(240,240,240), stop:1 rgb(223,223,223));"
        "}"
        "QPushButton:!enabled{"
        "   border: 1px solid rgb(210, 210, 210);"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 rgb(255,255,255), stop:1 rgb(238,238,238));"
        "}"
    ));

    if(_locked){
        _timer->stop();
        _locked = false;
    }
}

void ForwardButton::forward(){
    if(_playlistPlayer->mediaPlayer()->isPlaying() || _playlistPlayer->mediaPlayer()->isPaused()){
        _playlistPlayer->forward();
    }
}
