#ifndef ADVANCEDSETTINGSWINDOW_H
#define ADVANCEDSETTINGSWINDOW_H

#include <QDialog>

namespace Ui {
class AdvancedSettingsWindow;
}
class MediaSettings;
class AdvancedSettingsWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit AdvancedSettingsWindow(QWidget *parent = 0);
    ~AdvancedSettingsWindow();
    
private:
    Ui::AdvancedSettingsWindow *ui;
    MediaSettings* _mediaSettings;
};

#endif // ADVANCEDSETTINGSWINDOW_H
