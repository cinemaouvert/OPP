#ifndef BIN_H
#define BIN_H

#include <QWidget>

namespace Ui {
class Bin;
}

class Bin : public QWidget
{
    Q_OBJECT
    
public:
    explicit Bin(QWidget *parent = 0);
    ~Bin();
    
private:
    Ui::Bin *ui;
};

#endif // BIN_H
