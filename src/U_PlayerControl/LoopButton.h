#ifndef LOOPBUTTON_H
#define LOOPBUTTON_H

#include <QPushButton>

#include "PlaylistPlayer.h"

class LoopButton : public QPushButton
{
    Q_OBJECT
public:
    enum State
    {
        NOLOOP,
        BIGLOOP,
        SINGLELOOP
    };

    explicit LoopButton(PlaylistPlayer* playlistPlayer, QWidget *parent = 0);

private slots:
    void onClick();

private:
    State _state;
    PlaylistPlayer* _playlistPlayer;
};

#endif // LOOPBUTTON_H
