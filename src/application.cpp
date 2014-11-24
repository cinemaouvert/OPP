    #include "application.h"
#include <QMessageBox>

Application::Application(int & argc, char **argv):
    QApplication(argc, argv)
{
    // Get the settings
    QSettings settings("opp", "opp");

    /* Translation file is loading before the widgets contruction */
    _translator = new QTranslator();
    QString translationFile = "opp_";
    translationFile += settings.value("lang").toString();

    _translator->load(translationFile, applicationDirPath());
    installTranslator(_translator);

    // The main window
    _win = new MainWindow();

    /**
     * Set the library path to prevent the Qt default
     * behaviour which is to load the Qt dependencies
     * from the Qt folder installed on the computer.
     * Useful to deploy the software.
     */
    #if defined( Q_OS_MAC )
        QStringList libPaths;
        libPaths << applicationDirPath() + "/../PlugIns";
        QApplication::setLibraryPaths( libPaths );
    #endif

    #if (QT_VERSION >= 0x050000) // Qt version 5 and above
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    #else // until version 5
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    #endif

    /* Settings initialization */
    if(!settings.contains("moviesPath"))
        settings.setValue("moviesPath", QDir::homePath ());

    if(!settings.contains("updatePath"))
        settings.setValue("updatePath", config_opp::URL);

    if(!settings.contains("lang"))
    {
        /*Check if OS language is available, if not English is set as default language*/
        QString locale = QLocale::system().name().section('_', 0, 0);
        if(locale=="fr")
            settings.setValue("lang","fr");
        else
            settings.setValue("lang","en");
    }

    // The event filter
    installEventFilter(new CustomEventFilter(_win, this));

    /********* OPEN AN EXISTING LISTING **********/
    /** use size() function to be compatible with Qt4 and Qt5 */
    if(QApplication::arguments().size() > 1){ //Restart : filename en argument
       _win->openListing(QApplication::arguments()[1]);
    }

    /** use size() function to be compatible with Qt4 and Qt5 */
    if(QApplication::arguments().size() <= 1){ //Restart : filename en argument
        // Open the last used listing
        QSettings settings("opp", "opp");
        if(settings.contains("lastOpenedListing") && settings.contains("openLastUsedListing") && settings.value("openLastUsedListing").toBool()){
            if(!settings.value("lastOpenedListing").toString().isEmpty())
                _win->openListing(settings.value("lastOpenedListing").toString());
        }
    }
    /**********************************************/

    // The global message handler
    #if (QT_VERSION >= 0x050000) // Qt version 5 and above
        qInstallMessageHandler(MainWindow::myMessageHandler);
    #else // until version 5
        qInstallMsgHandler(MainWindow::myMessageHandler);
    #endif

    _win->show();
}

Application::~Application()
{
    delete _win;
    delete _translator;
}

bool Application::event(QEvent *event)
{
    switch (event->type()) {
        /**
         * This event is used on mac to open
         * an existing listing file when the user
         * make a double click on a .opp file
         */
        case QEvent::FileOpen:{
            QFileOpenEvent* e = static_cast<QFileOpenEvent *>(event);

            _win->openListing(e->file());

            return true;
        }
        default:{
            return QApplication::event(event);
        }
    }
}
