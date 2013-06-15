#ifndef PLAYBACK_H
#define PLAYBACK_H

#include <QObject>
#include "media.h"
#include "mediasettings.h"

class Playback : public QObject
{
    Q_OBJECT
public:
    explicit Playback(Media *media, QObject *parent = 0);


    virtual ~Playback();

    inline Media* media() const {return _media;}
    inline MediaSettings* mediaSettings() const {return _mediaSettings;}
    
private:
    Media *_media;
    MediaSettings *_mediaSettings;
    
};

#endif // PLAYBACK_H
