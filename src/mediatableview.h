#ifndef MEDIATABLEVIEW_H
#define MEDIATABLEVIEW_H

#include <QTableView>

class MediaTableView : public QTableView
{
    Q_OBJECT
public:
    explicit MediaTableView(QWidget *parent = 0);
    
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

private:
    void startDrag();
    QPoint startPos;
    
};

#endif // MEDIATABLEVIEW_H
