#include "playlisttableview.h"
#include "playlistmodel.h"
#include <QMouseEvent>
#include <QApplication>
#include <QItemSelectionModel>
#include <QDebug>

PlaylistTableView::PlaylistTableView(QWidget *parent) :
    QTableView(parent)
{
    setAcceptDrops(true);
}

void PlaylistTableView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        startPos = event->pos();
    }
    QTableView::mousePressEvent(event);
}

void PlaylistTableView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() && Qt::LeftButton)
    {
        int distance = (event->pos() - startPos).manhattanLength();
        if (distance >= QApplication::startDragDistance())
            startDrag(Qt::MoveAction);
    }
    QTableView::mouseMoveEvent(event);
}

void PlaylistTableView::startDrag(Qt::DropActions supportedActions)
{
    QModelIndexList indexes = selectionModel()->selectedRows();

    if (indexes.count() == 0)
        return;

    QModelIndex index = indexes.first();

    QMimeData *mimedata = new QMimeData;

    mimedata->setText(index.data().toString());

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimedata);

    if (drag->start(Qt::CopyAction | Qt::MoveAction) == Qt::MoveAction) {
        clearSelection();
    }
}

void PlaylistTableView::dragEnterEvent(QDragEnterEvent *event)
{
    PlaylistTableView *source = (PlaylistTableView *)(event->source());
    if (source && source != this)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void PlaylistTableView::dragMoveEvent(QDragMoveEvent *event)
{
    PlaylistTableView *source = (PlaylistTableView *)(event->source());
    if (source && source != this) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void PlaylistTableView::dropEvent(QDropEvent *event)
{
    model()->dropMimeData(event->mimeData(), event->dropAction(), 0, 0, indexAt(event->pos()));
    event->acceptProposedAction();

    QTableView::dropEvent(event);
}
