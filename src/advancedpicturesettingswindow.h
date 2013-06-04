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
    
private slots:
    void on_comboBox_deinterlace_currentIndexChanged(int index);

private:
    Ui::AdvancedPictureSettingsWindow *ui;
};

#endif // ADVANCEDPICTURESETTINGSWINDOW_H
