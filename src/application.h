#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>

class Project;

class Application : public QObject
{
    Q_OBJECT
    
public:
    /**
     * @brief Singleton instance getter
     * @return The singleton instance
     */
    static Application *const instance();

    ~Application();

    inline Project *const currentProject() const { return _currentProject; }

private:
    explicit Application(QObject *parent = 0);

    /**
     * @brief Singleton instance
     */
    static Application *s_instance;

    Project *_currentProject;
};

#endif // APPLICATION_H
