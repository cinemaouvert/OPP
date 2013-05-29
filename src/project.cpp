#include "project.h"

#include "application.h"
#include "mediaplayer.h"

Project::Project(QObject *parent) :
    QObject(parent)
{
}

Project::~Project()
{
}

void Project::saveToFile(const QString & location) const {

}

Project* Project::makeFromFile(const QString & location) {
    return 0;
}
