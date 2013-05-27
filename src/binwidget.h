#ifndef BINWIDGET_H
#define BINWIDGET_H

#include <QWidget>

namespace Ui {
class BinWidget;
}
class Media;
class Application;

class BinWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit BinWidget(QWidget *parent = 0);
    ~BinWidget();

public slots:
    void addMediaFromFileSystem();

signals:
    void mediaAdded(Media *const media);

private:
    Ui::BinWidget *ui;
    Application *_app;
};

#endif // BINWIDGET_H
