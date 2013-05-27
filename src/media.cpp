#include "media.h"

Media::Media(const QString &location, QObject *parent) :
    QObject(parent),
    _location(location)
{
}
