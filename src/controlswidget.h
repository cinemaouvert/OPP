#ifndef CONTROLSWIDGET_H
#define CONTROLSWIDGET_H

#include <QWidget>

namespace Ui {
class ControlsWidget;
}

class ControlsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ControlsWidget(QWidget *parent = 0);
    ~ControlsWidget();
    
private:
    Ui::ControlsWidget *ui;
};

#endif // CONTROLSWIDGET_H
