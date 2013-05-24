#ifndef BINWIDGET_H
#define BINWIDGET_H

#include <QWidget>

namespace Ui {
class BinWidget;
}
class Media;

class BinWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit BinWidget(QWidget *parent = 0);
    ~BinWidget();

signals:

    void mediaAdded(Media *);

private:
    Ui::BinWidget *ui;
};

#endif // BINWIDGET_H
