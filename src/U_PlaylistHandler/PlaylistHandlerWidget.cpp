#include "PlaylistHandlerWidget.h"

PlaylistHandlerWidget::PlaylistHandlerWidget(MainWindow* mw, QWidget* parent):
    QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->setContentsMargins(0, 1, 0, 1);

    _playlistTabWidget = new PlaylistTabWidget(mw);
    _playlistControlWidget = new PlaylistControlWidget(mw, this);

    layout->addLayout(_playlistControlWidget);
    layout->addWidget(_playlistTabWidget);
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
