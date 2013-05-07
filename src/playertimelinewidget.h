#ifndef PLAYERTIMELINEWIDGET_H
#define PLAYERTIMELINEWIDGET_H

#include <QWidget>

namespace Ui {
class PlayerTimelineWidget;
}
namespace opp {
class Playlist;
}

class PlayerTimelineWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PlayerTimelineWidget(QWidget *parent = 0);
    ~PlayerTimelineWidget();
   void setPlaylist(Playlist *);

public slots:
    void step();
    void reset();
    void next();
    void previous();

private:
    Ui::PlayerTimelineWidget *ui;

    Playlist *m_playlist;
};

#endif // PLAYERTIMELINEWIDGET_H
