#include "settingswindow.h"
#include "ui_settingswindow.h"
#include <QSettings>
#include <QString>
#include <QFileDialog>
#include <QDir>

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

void SettingsWindow::on_pushButton_VLCPath_clicked()
{

    QString pathVlc = QFileDialog::getOpenFileName(this,"", QDir::homePath(),"");
    if(pathVlc!="")
        ui->lineEdit_VLCPath->setText(pathVlc);
}

void SettingsWindow::on_pushButton_testPatternPath_clicked()
{
    QString pathTestPattern = QFileDialog::getOpenFileName(this,"", QDir::homePath(),"");
    if(pathTestPattern!="")
        ui->lineEdit_testPatternPath->setText(pathTestPattern);
}

void SettingsWindow::on_pushButton_intertitlePath_clicked()
{
    QString pathIntertitle = QFileDialog::getOpenFileName(this,"", QDir::homePath(),"");
    if(pathIntertitle!="")
        ui->lineEdit_intertitlePath->setText(pathIntertitle);
}

void SettingsWindow::on_pushButton_moviesPath_clicked()
{
    QString pathMovies = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::homePath(),QFileDialog::ShowDirsOnly);
    if(pathMovies!="")
        ui->lineEdit_moviesPath->setText(pathMovies);
}
