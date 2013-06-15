#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>

struct libvlc_instance_t;

class Application
{
public:
    Application();
    ~Application();

    /**
     * @brief Initialize the vlc instance from list of arguments
     * @param args The list of arguments
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    void initVlcInstanceFromArgs(const QStringList &args);

    /**
     * @brief vlcInstance
     * @param arg1 zofhzo
     * @return Description de ce que ca retourne si necessaire
     *
     * @author Florian Mhun <florian.mhun@gmail.com>
     */
    inline libvlc_instance_t* vlcInstance() const { return _vlcInstance; }

private:

    /**
     * @brief _vlcInstance The vlc instance
     */
    libvlc_instance_t *_vlcInstance;
};

#endif // APPLICATION_H
