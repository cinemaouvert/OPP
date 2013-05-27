#include "binwidget.h"
#include "ui_binwidget.h"

#include "application.h"
#include "project.h"
#include "media.h"

BinWidget::BinWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BinWidget)
{
    ui->setupUi(this);
    _app = Application::instance();

    //    _model = new MediaListModel(project->mediaList());
    //    ui->mediaListTableWidget->removeRow(0);

    // core connections
//    connect( _app->currentProject(), mediaAdded(Media*), this, addMedia() );

    // ui connections
//    connect( ui->addButton, clicked(), this, addMediaFromFileSystem());
}

BinWidget::~BinWidget()
{
    delete ui;
}

void BinWidget::addMediaFromFileSystem()
{
    Media *media = new Media("/path/to/file.mkv");
    if (_app->currentProject()->addMedia(media) == false) {
        delete media;
    } else {
//        QTableWidget *table = ui->mediaListTableWidget;

    }
}
