#include <QMouseEvent>
#include <QTimer>

#include "PlayerControlWidget.h"
#include "FlipBar.h"

FlipBar::FlipBar(QWidget *parent) :
    QSlider(parent),
    _mediaPlayer(0),
    _oldState("")
{
    _playerControlWidget = (PlayerControlWidget*)((SeekWidget*)parent)->parent();

    setOrientation(Qt::Horizontal);
    setMaximum(1);
    setStyleSheet(QString(
         "QSlider{"
         "  margin: 0px;"
         "}"
         "QSlider::groove:horizontal {"
         "  border: 1px solid rgb(153,153,153);"
         "  height: 8px;" /* the groove expands to the size of the slider by default. by giving it a height, it has a fixed size */
         "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(177,177,177), stop:1 rgb(196,196,196));"
         "  margin: 0px 0px 0px 0px;"
         "}"
        "QSlider::groove:horizontal:!enabled {"
        "  border: 1px solid rgb(173,173,173);"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(207,207,207), stop:1 rgb(226,226,226));"
        "}"
         "QSlider::handle:horizontal {"
         "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 rgb(240,240,240), stop:1 rgb(223,223,223));"
         "  border: 1px solid rgb(136, 136, 136);"
         "  width: 18px;"
         "  margin: 8px -1px 8px -1px;" /* handle is placed by default on the contents rect of the groove. Expand outside the groove */
         "}"
        "QSlider::handle:horizontal:!enabled {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 rgb(255,255,255), stop:1 rgb(238,238,238));"
        "  border: 1px solid rgb(156, 156, 156);"
        "}"
         "QSlider::sub-page:horizontal {"
         "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(146,146,146), stop:1 rgb(127,127,127));"
         "  height: 8px;"
         "  border: 1px solid rgb(113,113,113);"
         "  margin: 0px 0px 0px 0px;"
         "}"
        "QSlider::sub-page:horizontal:!enabled {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(176,176,176), stop:1 rgb(157,157,157));"
        "  border: 1px solid rgb(143,143,143);"
        "}"
    ));
}

void FlipBar::setPlaylistPlayer(PlaylistPlayer* playlistPlayer){
    _playlistPlayer = playlistPlayer;
    _mediaPlayer = playlistPlayer->mediaPlayer();
}


void FlipBar::mousePressEvent(QMouseEvent *event)
{
    event->ignore();

    if (!isEnabled() || !_mediaPlayer ){
        QSlider::mousePressEvent(event);
        return;
    }

    /*** allow to seek in the media even if the media is stopped ***/
    if(_mediaPlayer->isStopped()){
        _playlistPlayer->play();

        // we put pause after a while
        QTimer::singleShot(60, this, SLOT(pauseAfterAWhile()));
    }
    else if(_mediaPlayer->isPlaying()){
        _mediaPlayer->pause();
        _oldState = "playing";
    }

    /*** Down the sound ***/
    if(_mediaPlayer->volume() != 0)
        _mediaPlayer->setVolume(0);

    updatePosition(event->x());

    QSlider::mousePressEvent(event);
}

void FlipBar::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();

    if(_mediaPlayer->media() == NULL ){
        QSlider::mouseMoveEvent(event);
        return;
    }

    updatePosition(event->x());

    QSlider::mouseMoveEvent(event);
}

void FlipBar::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();

    if(_mediaPlayer->media() == NULL ){
        QSlider::mouseReleaseEvent(event);
        return;
    }

    updatePosition(event->x());

    if(_oldState == "playing"){
        _mediaPlayer->play();
        _oldState = "";
    }

    /*** Put back the sound volume ***/
    _mediaPlayer->setVolume(_playerControlWidget->currentVolume());

    QSlider::mouseReleaseEvent(event);
}

void FlipBar::updatePosition(int posX)
{
    int grooveWidth = width() - 2 - 16; // The global width less 2 (2 borders) - 16 (2 margins)
    float unit = maximum() / grooveWidth;
    float newValue = (posX - 9) * unit; // 9 = 1 margin + 1 border

    setValue(newValue);

    // Change to percentage
    newValue = newValue/maximum();
    _mediaPlayer->setPosition(newValue);

    // Stop the faders out
    _mediaPlayer->stopFaderOut(); // TEST DEBUG FIN SEEK BAR LECTURE

    emit positionManuallyChanged();
}

void FlipBar::pauseAfterAWhile()
{
    if(_mediaPlayer->isStopped())
        return;

    _mediaPlayer->pause();
}
