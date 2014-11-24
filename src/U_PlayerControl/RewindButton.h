#ifndef REWINDBUTTON_H
#define REWINDBUTTON_H

#include <QPushButton>
#include <QMouseEvent>
#include <QTimer>
#include "PlaylistPlayer.h"

class RewindButton : public QPushButton
{
    Q_OBJECT
public:
    explicit RewindButton(PlaylistPlayer* playlistPlayer, QWidget *parent = 0);
    ~RewindButton();

public slots:
    void rewind();

protected:
    void mousePressEvent(QMouseEvent *e);

    void mouseReleaseEvent(QMouseEvent *e);

private:
    PlaylistPlayer* _playlistPlayer;

    QTimer* _timer;

    bool _locked;
};

#endif // REWINDBUTTON_H
