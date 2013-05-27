#include "project.h"

Project::Project(QObject *parent) :
    QObject(parent)
{
}

void Project::saveToFile(const QString & location) const {

}

bool Project::addMedia(Media *media) {
    if (_mediaList.contains(media)) {
        return false;
    } else {
        _mediaList.append(media);
        return true;
    }
}

Project* Project::makeFromFile(const QString & location) {

}
