#ifndef PLAYERTIMELINEWIDGET_H
#define PLAYERTIMELINEWIDGET_H

#include <QWidget>
#include <QTime>

namespace Ui {
class PlayerTimelineWidget;
}
class Media;
class MediaPlayer;

class PlayerTimelineWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PlayerTimelineWidget(QWidget *parent = 0);
    ~PlayerTimelineWidget();

    void setMediaPlayer(MediaPlayer *media);

    void setNextMedia(Media *media);
    void setFutureMedia(Media *media);

private:
    Ui::PlayerTimelineWidget *ui;
    bool _paused;
    bool _lockAutoUpdate;

    MediaPlayer *_mediaPlayer;

    Media *_nextMedia;
    Media *_futurMedia;
};

#endif // PLAYERTIMELINEWIDGET_H
