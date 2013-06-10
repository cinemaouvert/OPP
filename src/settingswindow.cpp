#include "settingswindow.h"
#include "ui_settingswindow.h"
#include <QSettings>
#include <QString>

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    /*Settings*/
    QSettings settings("opp","opp");
    ui->lineEdit_VLCPath->setText(settings.value("vlcPath").toString());
    ui->lineEdit_testPatternPath->setText(settings.value("testPatternPath").toString());
    ui->lineEdit_intertitlePath->setText(settings.value("intertitlePath").toString());
    ui->lineEdit_moviesPath->setText(settings.value("moviesPath").toString());

    ui->comboBox_language->setCurrentIndex(getIndex(settings.value("lang").toString()));
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

int SettingsWindow::getIndex(QString lang)
{
    if(lang=="en")
        return 0;
    else if(lang=="fr")
        return 1;
}

QString SettingsWindow::getLang(int index)
{
    switch(index)
    {
        case 0:
            return "en";
            break;
        case 1:
            return "fr";
            break;
        default:
            return "en";
    }
}

void SettingsWindow::on_buttonBox_accepted()
{
    QSettings settings("opp","opp");
    settings.setValue("vlcPath", ui->lineEdit_VLCPath->text());
    settings.setValue("testPatternPath", ui->lineEdit_testPatternPath->text());
    settings.setValue("intertitlePath", ui->lineEdit_intertitlePath->text());
    settings.setValue("moviesPath", ui->lineEdit_moviesPath->text());
    settings.setValue("lang", getLang(ui->comboBox_language->currentIndex()));
    this->close();
}

void SettingsWindow::on_buttonBox_rejected()
{
    this->close();
}
