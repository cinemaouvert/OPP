#ifndef PLAYLISTDETAILSWIDGET_H
#define PLAYLISTDETAILSWIDGET_H


#include <QGridLayout>
#include <QFrame>

#include "mainwindow.h"
#include "PlaylistHandlerWidget.h"

class PlaylistHandlerWidget;
class PlaylistDetailsWidget: public QGridLayout
{
    Q_OBJECT
public:
    PlaylistDetailsWidget(MainWindow* mw, QWidget* parent = 0);
    virtual ~PlaylistDetailsWidget();
public slots:
    void updatePlaylistWidget();
    void updatePlaylistWidgetEndsInPlay();
    void updatePlaylistWidgetEndsInPause();
    void updatePlaylistWidgetEndsInStop();
    void updatePlaylistWidgetEndsInTimer();
    void updateEcart();

private:
    MainWindow* _mw;

    PlaylistHandlerWidget* _parent;

    int ecart;

    QLabel* _playlistNameLabel;

    QLabel* _playlistNameValue;

    QLabel* _playlistDurationLabel;

    QLabel* _playlistDurationValue;

    QLabel* _nbFilmsLabel;

    QLabel* _nbFilmsValue;

    QLabel* _playlistEndsInLabel;

    QLabel* _playlistEndsInValue;

    QTimer* _timer;
};

#endif // PLAYLISTDETAILSWIDGET_H
