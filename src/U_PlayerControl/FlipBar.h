#ifndef FLIPBAR_H
#define FLIPBAR_H

#include <QSlider>
#include <QTimer>

#include "MediaPlayer.h"
#include "PlayerControlWidget.h"

class PlayerControlWidget;

class FlipBar : public QSlider
{
    Q_OBJECT
public:
    explicit FlipBar(QWidget *parent = 0);

    void setPlaylistPlayer(PlaylistPlayer* mediaPlayer);

signals:
    void positionManuallyChanged();

protected:
    /**
     * @brief Change the elapsed time value and the remaining time value.
     * @param time The new time value
     */
    void mouseMoveEvent(QMouseEvent *event);

    /**
     * @brief flipBarReleased
     */
    void mouseReleaseEvent(QMouseEvent *event);

    /**
     * @brief mousePressEvent
     * @param event
     */
    void mousePressEvent(QMouseEvent *event);

    /**
     * @brief updatePosition
     */
    void updatePosition(int posX);

private slots:
    void pauseAfterAWhile();

private:

    PlaylistPlayer* _playlistPlayer;

    MediaPlayer* _mediaPlayer;

    PlayerControlWidget* _playerControlWidget;

    /**
     * @brief Allow to know if the madia player was stopped during a seek widget action.
     */
    QString _oldState;
};

#endif // FLIPBAR_H
