#include "playlistswidget.h"
#include "ui_playlistswidget.h"

PlaylistsWidget::PlaylistsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaylistsWidget)
{
    ui->setupUi(this);
}

PlaylistsWidget::~PlaylistsWidget()
{
    delete ui;
}
