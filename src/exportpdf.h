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
#ifndef EXPORTPDF_H
#define EXPORTPDF_H

#include <QDialog>

namespace Ui {
class ExportPDF;
}

class ExportPDF : public QDialog
{
    Q_OBJECT

public:
    explicit ExportPDF(QWidget *parent = 0);
    ~ExportPDF();

    /**
     * @brief set html
     * @param html the html text
     *
     * @author Denis Saunier <saunier.denis.86@gmail.com>
     */
    void setHtml(const QString &html);

    /**
     * @brief set html
     * @param html the html text
     *
     * @author Denis Saunier <saunier.denis.86@gmail.com>
     */
    void setHtmlPDF(const QString &pdf);

private slots:
    /**
     * @brief get schedule and print pdf
     *
     * @author Denis Saunier <saunier.denis.86@gmail.com>
     */
    void on_exportPDF_Button_clicked();

    /**
     * @brief hide window
     *
     * @author Denis Saunier <saunier.denis.86@gmail.com>
     */
    void on_CancelButton_clicked();

    void on_pushButton_ExportHtml_clicked();

private:
    /**
     * @brief The ui
     */
    Ui::ExportPDF *ui;

    /**
     * @brief The destination folder
     */
    QString _dir;

    /**
     * @brief The html text
     */
    QString _html;

    /**
     * @brief The html text for pdf
     */
    QString _pdf;
};

#endif // EXPORTPDF_H
