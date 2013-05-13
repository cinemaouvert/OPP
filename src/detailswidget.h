#ifndef DETAILSWIDGET_H
#define DETAILSWIDGET_H

#include <QWidget>

namespace Ui {
class DetailsWidget;
}

class DetailsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit DetailsWidget(QWidget *parent = 0);
    ~DetailsWidget();
    
private:
    Ui::DetailsWidget *ui;
};

#endif // DETAILSWIDGET_H
