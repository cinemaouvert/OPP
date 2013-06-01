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
    Playback(const Playback &playback);
    Playback& operator =(const Playback &playback);


    virtual ~Playback();

    inline Media* media() const {return _media;}
    inline const MediaSettings& mediaSettings() const {return _mediaSettings;}
    
private:
    Media *_media;
    MediaSettings _mediaSettings;
    
};

#endif // PLAYBACK_H
