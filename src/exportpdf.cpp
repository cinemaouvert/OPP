/**********************************************************************************
 * This file is part of Open Projection Program (OPP).
 *
 * Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
 *
 * Authors: Denis Saunier <saunier.denis.86@gmail.com>
 *          Geoffrey Bergé <geoffrey.berge@live.fr>
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

void ExportPDF::setHtmlPDF(const QString &pdf){
    _pdf = pdf;
    ui->textEdit_viewPDF->setHtml(_pdf.toUtf8());
}

void ExportPDF::on_exportPDF_Button_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Schedule"), "", tr("PDF file (*.pdf)"));

    if (fileName.isEmpty()) {
        return;
    } else {
        if(fileName.right(4)!=".pdf")
            fileName+=".pdf";

        QApplication::setOverrideCursor(Qt::WaitCursor);
        QPrinter printer(QPrinter::HighResolution);
                  printer.setOutputFormat(QPrinter::PdfFormat);
                  printer.setOrientation(QPrinter::Landscape);
                  printer.setOutputFileName(fileName);

         QWebView webView;
         webView.setHtml(_html);
         webView.print(&printer);
         webView.close();
         _pdf = "";
         _dir = "";
         QApplication::restoreOverrideCursor();
     }
     this->hide();
}

void ExportPDF::on_CancelButton_clicked()
{
    _html = "";
    _dir = "";
    this->hide();

}

void ExportPDF::on_pushButton_ExportHtml_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Schedule"), "", tr("HTML file (*.html)"));

    if (fileName.isEmpty()) {
        return;
    } else {
        if(fileName.right(5)!=".html")
            fileName+=".html";

        QFile htmlFile(fileName);
        htmlFile.open(QIODevice::WriteOnly);
        htmlFile.write(_html.toUtf8());
        htmlFile.close();
        _html = "";
        _dir = "";
        QApplication::restoreOverrideCursor();
     }
     this->hide();
}
