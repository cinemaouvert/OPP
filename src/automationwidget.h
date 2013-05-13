#ifndef AUTOMATIONWIDGET_H
#define AUTOMATIONWIDGET_H

#include <QWidget>

namespace Ui {
class AutomationWidget;
}

class AutomationWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit AutomationWidget(QWidget *parent = 0);
    ~AutomationWidget();
    
private:
    Ui::AutomationWidget *ui;
};

#endif // AUTOMATIONWIDGET_H
