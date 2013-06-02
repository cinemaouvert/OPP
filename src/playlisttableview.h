#ifndef PLAYLISTTABLEVIEW_H
#define PLAYLISTTABLEVIEW_H

#include <QTableView>

class PlaylistTableView : public QTableView
{
    Q_OBJECT
public:
    explicit PlaylistTableView(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void startDrag(Qt::DropActions supportedActions);

private:
    QPoint startPos;

};

#endif // PLAYLISTTABLEVIEW_H
