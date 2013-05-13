#ifndef RESUMEDETAILSWIDGET_H
#define RESUMEDETAILSWIDGET_H

#include <QWidget>

namespace Ui {
class ResumeDetailsWidget;
}

class ResumeDetailsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ResumeDetailsWidget(QWidget *parent = 0);
    ~ResumeDetailsWidget();
    
private:
    Ui::ResumeDetailsWidget *ui;
};

#endif // RESUMEDETAILSWIDGET_H
