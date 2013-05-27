#include "binwidget.h"
#include "ui_binwidget.h"

#include <QFileDialog>
#include <QDebug>

#include "application.h"
#include "project.h"
#include "media.h"

#include "medialistmodel.h"

BinWidget::BinWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BinWidget)
{
    ui->setupUi(this);
    _app = Application::instance();
    _mediaListModel = new MediaListModel(_app->currentProject()->mediaList());

    ui->mediaTableView->setModel(_mediaListModel);

//    model->addMedia(new Media("/path/to/movie.mkv"));

//    ui->mediaListTableWidget->removeRow(0);

    // core connections
//    connect( _app->currentProject(), mediaAdded(Media*), this, addMedia() );

    // ui connections
//    connect( ui->addButton, clicked(), this, addMediaFromFileSystem());
//    connect(ui->addButton, clicked(bool), this, on_addMediaButton_clicked());
}

BinWidget::~BinWidget()
{
    delete ui;
}

//void BinWidget::addMediaFromFileSystem()
//{
//    Media *media = new Media("/path/to/file.mkv");
//    if (_app->currentProject()->addMedia(media) == false) {
//        delete media;
//    } else {
////        QTableWidget *table = ui->mediaListTableWidget;

//    }
//}

void BinWidget::on_addMediaButton_clicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("New media"), "/Users/floomoon", tr("Media (*.avi *.mkv *.jpg *.png)"));

    foreach (QString fileName, fileNames) {
        Media media(fileName);
        if (media.exists() == false) {
            // error: media file not exists
        }
        _mediaListModel->addMedia(media);
    }
}

void BinWidget::on_deleteMediaButton_clicked()
{
    QItemSelectionModel *selectionModel = ui->mediaTableView->selectionModel();
    QModelIndexList indexes = selectionModel->selectedRows();

    foreach (QModelIndex index, indexes) {
        _mediaListModel->removeRows(index.row(), 1);
    }
}
