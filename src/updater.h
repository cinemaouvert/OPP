#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <QtCore/QUrl>
#include <QtCore/QFile>
#include <QEventLoop>
#include <QMessageBox>
#include <QProcess>
#include <QDesktopServices>

#include "config.h"

/**
 * @brief Base class for update application
 */
class Updater : public QObject
{
    Q_OBJECT
public:
    explicit Updater(QObject *parent = 0);
    virtual ~Updater();

    /**
     * @brief Compare version software with installer on cinemaouvert.fr
     *
     * @author Denis Saunier <saunier.denis.86@gmail.com>
     */
    void checkVersion();

private:
    /**
     * @brief Download installer
     *
     * @author Denis Saunier <saunier.denis.86@gmail.com>
     */
    void getInstaller();
};


#endif // UPDATER_H
