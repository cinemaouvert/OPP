#include "mediatableview.h"
#include "medialistmodel.h"
#include <QMouseEvent>
#include <QApplication>
#include <QItemSelectionModel>
#include <QDebug>

MediaTableView::MediaTableView(QWidget *parent) :
    QTableView(parent)
{
    setAcceptDrops(true);
}

void MediaTableView::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        startPos=event->pos();
    }
    QTableView::mousePressEvent(event);
}

void MediaTableView::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons()&&Qt::LeftButton)
    {
        int distance=(event->pos()-startPos).manhattanLength();
        if(distance>=QApplication::startDragDistance())
            startDrag();
    }
    QTableView::mouseMoveEvent(event);
}

void MediaTableView::startDrag()
{
    QModelIndexList indexes = this->selectionModel()->selectedRows(MediaListModel::Location);
    if(indexes.count()==0)
        return;
    QModelIndex index=indexes.first();
    QMimeData *mimedata = new QMimeData;
    mimedata->setText(index.data().toString());
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimedata);
    drag->start(Qt::MoveAction);

}

void MediaTableView::dragEnterEvent(QDragEnterEvent *event)
{
    MediaTableView *source = (MediaTableView *)(event->source());
    if(source)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void MediaTableView::dragMoveEvent(QDragMoveEvent *event)
{
    MediaTableView *source = (MediaTableView *)(event->source());
    if(source)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void MediaTableView::dropEvent(QDropEvent *event)
{
}
