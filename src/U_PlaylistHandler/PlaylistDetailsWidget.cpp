#include "PlaylistDetailsWidget.h"
#include "playback.h"

PlaylistDetailsWidget::PlaylistDetailsWidget(MainWindow* mw, QWidget* parent):
    QGridLayout()
{

    _mw = mw;
    _parent = (PlaylistHandlerWidget*) parent;

    _playlistNameLabel = new QLabel(tr("Playlist being played :"));
    _playlistDurationLabel = new QLabel(tr("Duration :"));
    _nbFilmsLabel = new QLabel(tr("Number of films :"));
    _playlistEndsInLabel = new QLabel(tr("Ends in :"));
    _playlistNameValue = new QLabel("-");
    _playlistDurationValue = new QLabel("--:--:--:");
    _nbFilmsValue = new QLabel("0");
    _playlistEndsInValue = new QLabel("--:--:--");
    _timer = new QTimer(this);

    addWidget(_playlistNameLabel,0,0);
    addWidget(_playlistNameValue,0,1);
    addWidget(_playlistDurationLabel,0,2);
    addWidget(_playlistDurationValue,0,3);
    addWidget(_nbFilmsLabel,1,0);
    addWidget(_nbFilmsValue,1,1);
    addWidget(_playlistEndsInLabel,1,2);
    addWidget(_playlistEndsInValue,1,3);

    _playlistEndsInLabel->hide();
    _playlistEndsInValue->hide();

    connect(_mw->playerControlWidget(), SIGNAL(playState()), this, SLOT(updatePlaylistWidgetEndsInPlay()));
    connect(_mw->playerControlWidget(), SIGNAL(pauseState()), this, SLOT(updatePlaylistWidgetEndsInPause()));
    connect(_mw->playerControlWidget(), SIGNAL(stopState()), this, SLOT(updatePlaylistWidgetEndsInStop()));
    connect(_mw->playerControlWidget()->seekWidget()->flipBar(), SIGNAL(positionManuallyChanged()), this, SLOT(updateEcart()));


}

PlaylistDetailsWidget::~PlaylistDetailsWidget()
{
}


void PlaylistDetailsWidget::updatePlaylistWidget()
{
    _playlistNameValue->setText(_mw->playlistPlayer()->currentPlaylist()->title());
    _playlistDurationValue->setText(QString(msecToQTime(_mw->playlistPlayer()->currentPlaylist()->totalDuration()).toString("hh:mm:ss")));
    _nbFilmsValue->setText(QString::number(_mw->playlistPlayer()->currentPlaylist()->playbackList().count()));
}


void PlaylistDetailsWidget::updatePlaylistWidgetEndsInPlay()
{
    updateEcart();
    connect(_timer, SIGNAL(timeout()), this, SLOT(updatePlaylistWidgetEndsInTimer()));
    _timer->start(1000);

    _playlistEndsInLabel->show();
    _playlistEndsInValue->show();


}

void PlaylistDetailsWidget::updatePlaylistWidgetEndsInTimer()
{
    ecart-=1;
    QTime t = QTime(0,0);
    t = t.addSecs(ecart);
    _playlistEndsInValue->setText(t.toString("hh:mm:ss"));
}

void PlaylistDetailsWidget::updatePlaylistWidgetEndsInPause()
{
        _playlistEndsInLabel->show();
        _playlistEndsInValue->show();
        _timer->stop();
        disconnect(_timer, SIGNAL(timeout()), this, SLOT(updatePlaylistWidgetEndsInTimer()));

}
void PlaylistDetailsWidget::updatePlaylistWidgetEndsInStop()
{
        _playlistEndsInLabel->hide();
        _playlistEndsInValue->hide();
        _timer->stop();
         disconnect(_timer, SIGNAL(timeout()), this, SLOT(updatePlaylistWidgetEndsInTimer()));
}

void PlaylistDetailsWidget::updateEcart(){
    ecart = QTime(0,0,0,0).msecsTo(_mw->playlistPlayer()->mediaPlayer()->currentPlayback()->media()->remainingTime());
    if(ecart == 0)
    {
        ecart = _mw->playlistPlayer()->mediaPlayer()->currentPlayback()->media()->duration();
    }
    int index =  _mw->playlistPlayer()->currentPlaylist()->indexOf(_mw->playlistPlayer()->mediaPlayer()->currentPlayback());
    for(int i=index+1;i<_mw->playlistPlayer()->currentPlaylist()->count(); i++)
    {
        ecart = ecart + _mw->playlistPlayer()->currentPlaylist()->at(i)->media()->duration();
    }
    ecart = ecart/1000;
}
