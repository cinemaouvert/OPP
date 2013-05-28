#include "project.h"

#include "application.h"
#include "mediaplayer.h"

Project::Project(QObject *parent) :
    QObject(parent),
    _mediaPlayer(new MediaPlayer)
{
}

Project::~Project()
{
    delete _mediaPlayer;
}

void Project::saveToFile(const QString & location) const {

}

Project* Project::makeFromFile(const QString & location) {

}
