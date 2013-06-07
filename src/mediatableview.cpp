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
    qDebug()<<"media start drag";
    QModelIndexList indexes = selectionModel()->selectedRows();

    if (indexes.count() == 0)
        return;

    QString paths;
QModelIndex index;
    foreach (index, indexes) {
        paths += QString::number(index.row());
        paths += ":";
    }

    QMimeData *mimedata = new QMimeData;
    mimedata->setText("drop your media to the playlist");
    mimedata->setHtml(paths.toUtf8());

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimedata);

    if (drag->start(Qt::CopyAction | Qt::MoveAction) == Qt::MoveAction) {
        qDebug()<<"<<< END DRAG >>>>";
        selectionModel()->clear();
    }
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
