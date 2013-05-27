#ifndef BINWIDGET_H
#define BINWIDGET_H

#include <QWidget>

namespace Ui {
class BinWidget;
}
class Media;
class Application;
class MediaListModel;

class BinWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit BinWidget(QWidget *parent = 0);
    ~BinWidget();

signals:
    void mediaAdded(Media *const media);

private slots:
    void on_addMediaButton_clicked();
    void on_deleteMediaButton_clicked();

private:
    Ui::BinWidget *ui;
    Application *_app;
    MediaListModel *_mediaListModel;
};

#endif // BINWIDGET_H
