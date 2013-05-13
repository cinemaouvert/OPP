#ifndef SATURATIONWIDGET_H
#define SATURATIONWIDGET_H

#include <QWidget>

namespace Ui {
class SaturationWidget;
}

class SaturationWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SaturationWidget(QWidget *parent = 0);
    ~SaturationWidget();
    
private:
    Ui::SaturationWidget *ui;
};

#endif // SATURATIONWIDGET_H
