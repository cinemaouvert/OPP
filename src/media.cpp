#include "media.h"

Media::Media(const QString &location, QObject *parent) :
    QObject(parent),
    _location(location)
{
    _fileInfo = QFileInfo(location);
}

QString Media::name() const
{
    return _fileInfo.fileName();
}

bool Media::isExists() const {
    return _fileInfo.exists();
}
