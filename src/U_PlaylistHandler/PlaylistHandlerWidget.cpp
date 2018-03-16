#include "PlaylistHandlerWidget.h"
#include <QGroupBox>

PlaylistHandlerWidget::PlaylistHandlerWidget(MainWindow* mw, QWidget* parent):
    QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    QHBoxLayout* horizontalLayoutTop = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->setContentsMargins(0, 1, 0, 1);

    _playlistTabWidget = new PlaylistTabWidget(mw);
    _playlistControlWidget = new PlaylistControlWidget(mw, this);
    _playlistDetailsWidget = new PlaylistDetailsWidget(mw, this);

    QGroupBox* myWidget = new QGroupBox;
    myWidget->setLayout(_playlistDetailsWidget);
    myWidget->setStyleSheet("margin-top:5;");
    horizontalLayoutTop->addLayout(_playlistControlWidget);
    horizontalLayoutTop->addWidget(_playlistTabWidget);
    layout->addLayout(horizontalLayoutTop);
    layout->addWidget(myWidget);

}

PlaylistTableView* PlaylistHandlerWidget::currentPlaylistTableView() const
{
    return (PlaylistTableView*) _playlistTabWidget->currentWidget();
}

PlaylistModel* PlaylistHandlerWidget::currentPlaylistModel() const
{
    return (PlaylistModel*) currentPlaylistTableView()->model();
}

Playlist*  PlaylistHandlerWidget::playlistAt(int index) const
{
    return ( (PlaylistModel*) ( (PlaylistTableView*) _playlistTabWidget->widget(index) )->model() )->playlist();
}
