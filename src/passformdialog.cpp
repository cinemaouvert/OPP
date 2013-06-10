#include "passformdialog.h"
#include "ui_passformdialog.h"
#include "locker.h"


passformdialog::passformdialog(Locker *locker, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::passformdialog)
{
    ui->setupUi(this);
    _locker = locker;
}

passformdialog::~passformdialog()
{
    delete ui;
}

QString passformdialog::getPass()
{
    return _lePass;
}

void passformdialog::on_pushButton_clicked()
{
    //enable or disable widgets
    if(this->getPass() == _locker->getPassword())
    {
        foreach(QWidget *widget,_locker->getWidgets())
        {
            widget->setEnabled(true);
        }
        this->close();
        //change button lock name
        /*if(_locker->getWidgets().first()->isEnabled())
        {
           // ui->lockButton->setText("Lock");
        }else{
           // ui->lockButton->setText("Unlock");
        }*/
    }

}

void passformdialog::on_enterPass_editingFinished()
{
    _lePass = ui->enterPass->text();
}
