#include "screenshotselector.h"
#include "ui_screenshotselector.h"
#include "utils.h"
#include "mainwindow.h"

#include <QDir>

ScreenshotSelector::ScreenshotSelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScreenshotSelector)
{
    ui->setupUi(this);

    vlc = libvlc_new(0, NULL);
    vlcp = libvlc_media_player_new(vlc);
}

ScreenshotSelector::~ScreenshotSelector()
{
    libvlc_release(vlc);
    libvlc_media_player_release(vlcp);

    delete ui;
}

void ScreenshotSelector::setMedia(Media *media){
    this->_media = media;

    ui->startLabel->setText("00:00:00:000");
    uint duration = this->_media->duration();
    QTime durationTime = msecToQTime(duration);
    ui->endLabel->setText(durationTime.toString("hh:mm:ss:zzz"));
    ui->seekSlider->setMinimum(1);
    ui->seekSlider->setMaximum(duration-1);
    ui->seekSlider->setPageStep(ui->stepInput->value());
    ui->seekSlider->setValue(1);



    _currentWId = ui->videoWidget->request();

    if (_currentWId) {
        #if defined(Q_OS_WIN)
                libvlc_media_player_set_hwnd(vlcp, (void *)_currentWId);
        #elif defined(Q_OS_MAC)
                libvlc_media_player_set_nsobject(vlcp, (void *)_currentWId);
        #elif defined(Q_OS_UNIX)
                libvlc_media_player_set_xwindow(vlcp, _currentWId);
        #endif
    }


    libvlc_media_player_set_media(vlcp, _media->core());
    libvlc_media_add_option(_media->core(),":noaudio");


    libvlc_media_player_play(vlcp);
    wait(750);
    libvlc_media_player_pause(vlcp);




}

void ScreenshotSelector::on_buttonBox_accepted()
{
    QString screenPath = "./screenshot/";
    screenPath = screenPath.replace("/",QDir::separator());
    screenPath +=  _media->getLocation().replace(QDir::separator(),"_").remove(":");
    screenPath += ".png";

    float width = libvlc_video_get_width(vlcp);
    float height = libvlc_video_get_height(vlcp);

    libvlc_video_take_snapshot(vlcp, 0, screenPath.toStdString().c_str(), width,height);

    ((MainWindow*) this->parent())->updateCurrentScreenshot();
    close();
}

void ScreenshotSelector::on_buttonBox_rejected()
{
    close();
}

void ScreenshotSelector::closeEvent (QCloseEvent *event){
    close();
}

void ScreenshotSelector::close(){
    libvlc_media_player_stop(vlcp);
}

void ScreenshotSelector::setScreenshot(int position){
    libvlc_media_player_set_time(vlcp,position);
}


void ScreenshotSelector::on_seekSlider_sliderReleased()
{
    int position = ui->seekSlider->value();
    setScreenshot(position);
    QTime durationTime = msecToQTime(position);
    ui->startLabel->setText(durationTime.toString("hh:mm:ss:zzz"));
}


void ScreenshotSelector::on_pushButton_clicked()
{
    ui->seekSlider->setValue(ui->seekSlider->value()-ui->seekSlider->pageStep());
    on_seekSlider_sliderReleased();
}

void ScreenshotSelector::on_pushButton_2_clicked()
{
    ui->seekSlider->setValue(ui->seekSlider->value()+ui->seekSlider->pageStep());
    on_seekSlider_sliderReleased();
}



void ScreenshotSelector::on_stepInput_valueChanged(int arg1)
{
    ui->seekSlider->setPageStep(arg1);
}
