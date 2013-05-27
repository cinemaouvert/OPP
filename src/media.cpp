#include "media.h"

Media::Media(const QString &location, QObject *parent) :
    QObject(parent),
    _location(location)
{
    _fileInfo = QFileInfo(location);
}

Media::Media(const Media &media)
{
    _location = media.location();
    _fileInfo = QFileInfo(_location);
}

Media & Media::operator=(const Media &media) {
    if (this != &media) {
        _location = media.location();
        _fileInfo = QFileInfo(_location);
    }
    return *this;
}

QString Media::name() const
{
    return _fileInfo.fileName();
}

bool Media::exists() const {
    return _fileInfo.exists();
}

bool Media::operator==(const Media &media) const
{
    return _location == media.location();
}
