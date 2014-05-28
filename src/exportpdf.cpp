/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Denis Saunier <saunier.denis.86@gmail.com>
 *
 * Open Projection Program is an initiative of Catalogue Ouvert du Cinéma.
 * The software was developed by four students of University of Poitiers
 * as school project.
 *
 * Open Projection Program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Open Projection Program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Open Projection Program. If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************************/

#include "exportpdf.h"
#include "ui_exportpdf.h"
#include <QFileDialog>
#include <QDateTime>
#include <QTextDocument>
#include <QPrinter>
#include <QDebug>
#include <QtWebKit/QWebView>

ExportPDF::ExportPDF(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportPDF)
{
    ui->setupUi(this);
    _dir = "";
    _html = "";
}

ExportPDF::~ExportPDF()
{
    delete ui;
}

void ExportPDF::setHtml(const QString &html){
    _html = html;
    ui->textEdit_viewPDF->setHtml(_html.toUtf8());
}

void ExportPDF::on_exportPDF_Button_clicked()
{
    _dir = QFileDialog::getExistingDirectory(this, tr("Open Folder"),
                                                 QCoreApplication::applicationDirPath(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QPrinter printer(QPrinter::HighResolution);
              printer.setOutputFormat(QPrinter::PdfFormat);
              printer.setOrientation(QPrinter::Landscape);
              printer.setOutputFileName(_dir + QDir::separator() +"scheduleOPP_" + (QDateTime::currentDateTime().toString()).replace(':', "_").replace('.', "_").replace(' ', "_") + ".pdf");

     QWebView webView;
     webView.setHtml(_html);
     webView.print(&printer);
     _html = "";
     _dir = "";
     QApplication::restoreOverrideCursor();
     this->hide();
}

void ExportPDF::on_CancelButton_clicked()
{
    _html = "";
    _dir = "";
    this->hide();

}
