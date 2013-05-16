#ifndef ADVANCEDPICTURESETTINGSWINDOW_H
#define ADVANCEDPICTURESETTINGSWINDOW_H

#include <QMainWindow>

namespace Ui {
class AdvancedPictureSettingsWindow;
}

class AdvancedPictureSettingsWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit AdvancedPictureSettingsWindow(QWidget *parent = 0);
    ~AdvancedPictureSettingsWindow();
    
private:
    Ui::AdvancedPictureSettingsWindow *ui;
};

#endif // ADVANCEDPICTURESETTINGSWINDOW_H
