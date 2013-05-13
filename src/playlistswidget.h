#ifndef PLAYLISTSWIDGET_H
#define PLAYLISTSWIDGET_H

#include <QWidget>

namespace Ui {
class PlaylistsWidget;
}

class PlaylistsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PlaylistsWidget(QWidget *parent = 0);
    ~PlaylistsWidget();
    
private:
    Ui::PlaylistsWidget *ui;
};

#endif // PLAYLISTSWIDGET_H
