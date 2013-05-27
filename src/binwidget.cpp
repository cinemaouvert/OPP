#include "binwidget.h"
#include "ui_binwidget.h"

#include "application.h"

BinWidget::BinWidget(QWidget *parent) :
    AbstractOppWidget(parent),
    ui(new Ui::BinWidget)
{
    ui->setupUi(this);
    _app = Application::instance();

    //    _model = new MediaListModel(project->mediaList());
    //    ui->mediaListTableWidget->removeRow(0);

    // core connections
    connect( _app->currentProject(), mediaAdded(Media*), this, addMedia(Media*) );

    // ui connections
    connect( ui->addButton, clicked(), this, addMediaFromFileSystem());
}

BinWidget::~BinWidget()
{
    delete ui;
}

void BinWidget::addMediaFromFileSystem()
{
    Media *media = new Media();
    if (_app->currentProject()->addMedia(media) == false) {
        delete media;
    } else {
//        QTableWidget *table = ui->mediaListTableWidget;

    }
}
