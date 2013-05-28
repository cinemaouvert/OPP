#include "binwidget.h"
#include "ui_binwidget.h"

#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

#include "application.h"
#include "project.h"
#include "media.h"

#include "medialistmodel.h"

BinWidget::BinWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BinWidget)
{
    ui->setupUi(this);

    //Application *app = Application::instance();
    //_mediaListModel = new MediaListModel(app->currentProject()->mediaList());

    //ui->mediaTableView->setModel(_mediaListModel);
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

//    foreach (QString fileName, fileNames) {
//        Media media(fileName, app.vlcInstance());
//        if (media.exists() == false) {
//            // error: media file not exists
//        }
//        if (_mediaListModel->addMedia(media) == false) {
//            QMessageBox msgBox;
//            msgBox.setIcon(QMessageBox::Warning);
//            msgBox.setText(QString("The file %1 was already imported.").arg(media.location()));
//            msgBox.exec();
//        }
//    }
}

void BinWidget::on_deleteMediaButton_clicked()
{
    QItemSelectionModel *selectionModel = ui->mediaTableView->selectionModel();
    QModelIndexList indexes = selectionModel->selectedRows();

    foreach (QModelIndex index, indexes) {
        _mediaListModel->removeRows(index.row(), 1);
    }
}
