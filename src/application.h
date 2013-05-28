#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>

struct libvlc_instance_t;

class Application
{
public:
    Application();
    ~Application();

    void initVlcInstanceFromArgs(const QStringList &args);

    inline libvlc_instance_t* vlcInstance() const { return _vlcInstance; }

private:
    libvlc_instance_t *_vlcInstance;
};

#endif // APPLICATION_H
