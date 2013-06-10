#ifndef PASSFORMDIALOG_H
#define PASSFORMDIALOG_H

#include <QDialog>

class Locker;

namespace Ui {
class passformdialog;
}


class PlaylistTableView;

class passformdialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit passformdialog(Locker *locker,QWidget *parent = 0);

    QString getPass();

    ~passformdialog();
    
private slots:
    void on_pushButton_clicked();

    void on_enterPass_editingFinished();


private:
    Ui::passformdialog *ui;
    QString _lePass;
    Locker *_locker;
   // QList<QWidget*> _widgets;
};

#endif // PASSFORMDIALOG_H
