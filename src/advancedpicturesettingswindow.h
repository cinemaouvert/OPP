#ifndef ADVANCEDPICTURESETTINGSWINDOW_H
#define ADVANCEDPICTURESETTINGSWINDOW_H

#include <QDialog>

namespace Ui {
class AdvancedPictureSettingsWindow;
}

class AdvancedPictureSettingsWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit AdvancedPictureSettingsWindow(QWidget *parent = 0);
    ~AdvancedPictureSettingsWindow();
    
private:
    Ui::AdvancedPictureSettingsWindow *ui;
};

#endif // ADVANCEDPICTURESETTINGSWINDOW_H
