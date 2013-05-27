#include "application.h"

#include "project.h"

Application *Application::s_instance = NULL;

Application::Application(QObject *parent) :
    QObject(parent)
{
    _currentProject = new Project;
}

Application::~Application()
{
    delete _currentProject;
    s_instance = NULL;
}

Application *const Application::instance()
{
    if (s_instance == NULL) {
        s_instance = new Application;
    }
    return s_instance;
}
