#ifndef PLAYERTIMELINEWIDGET_H
#define PLAYERTIMELINEWIDGET_H

#include <QWidget>

namespace Ui {
class PlayerTimelineWidget;
}

class PlayerTimelineWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PlayerTimelineWidget(QWidget *parent = 0);
    ~PlayerTimelineWidget();

private:
    Ui::PlayerTimelineWidget *ui;
};

#endif // PLAYERTIMELINEWIDGET_H
