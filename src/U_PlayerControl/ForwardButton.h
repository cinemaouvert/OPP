#ifndef FORWARDBUTTON_H
#define FORWARDBUTTON_H

#include <QPushButton>
#include <QMouseEvent>
#include <QTimer>
#include "PlaylistPlayer.h"

class ForwardButton : public QPushButton
{
    Q_OBJECT

public:
    explicit ForwardButton(PlaylistPlayer* playlistPlayer, QWidget *parent = 0);
    ~ForwardButton();

public slots:
    void forward();

protected:
    void mousePressEvent(QMouseEvent *e);

    void mouseReleaseEvent(QMouseEvent *e);

private:
    PlaylistPlayer* _playlistPlayer;

    QTimer* _timer;

    bool _locked;
};

#endif // FORWARDBUTTON_H
