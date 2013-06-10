#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QWidget>

namespace Ui {
class StatusWidget;
}

class StatusWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit StatusWidget(QWidget *parent = 0);
    ~StatusWidget();
    
public slots:
    void setMediaCount(int count);

protected:
    void timerEvent(QTimerEvent *event);

private:
    Ui::StatusWidget *ui;

    int _timerId;
};

#endif // STATUSWIDGET_H
