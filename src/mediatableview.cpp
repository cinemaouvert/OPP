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
    qDebug() << "media mouse press";
    if (event->button() == Qt::LeftButton) {
        startPos = event->pos();
    }
    QTableView::mousePressEvent(event);
}

void MediaTableView::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "media mouse move";
    if (event->buttons() && Qt::LeftButton) {
        int distance = (event->pos() - startPos).manhattanLength();
        if (distance >= QApplication::startDragDistance())
            startDrag(Qt::MoveAction);
    }
    QTableView::mouseMoveEvent(event);
}

void MediaTableView::startDrag(Qt::DropActions supportedActions)
{
    qDebug() << "media start drag";
    QModelIndexList indexes = this->selectionModel()->selectedRows(MediaListModel::Location);

    if (indexes.count()==0)
        return;

    QString paths;
    foreach (QModelIndex index, indexes) {
        paths+=index.data().toString();
        paths+="#***#";
    }

    QMimeData *mimedata = new QMimeData;
    mimedata->setText(paths);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimedata);
    drag->start(Qt::MoveAction);
}

void MediaTableView::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "media drag enter";
    MediaTableView *source = (MediaTableView *)(event->source());

    if (source) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void MediaTableView::dragMoveEvent(QDragMoveEvent *event)
{
    qDebug() << "media drag move";
    MediaTableView *source = (MediaTableView *)(event->source());

    if (source) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void MediaTableView::dropEvent(QDropEvent *event)
{
    qDebug() << "media drop";
}
