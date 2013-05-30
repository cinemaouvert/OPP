#include "mediasettings.h"

MediaSettings::MediaSettings(QObject *parent) :
    QObject(parent)
{
}

MediaSettings::MediaSettings(MediaSettings &mediasettings)
{

}

MediaSettings& MediaSettings::operator =(const MediaSettings &mediasettings)
{
    if(this!=&mediasettings)
    {

    }
    return *this;
}
