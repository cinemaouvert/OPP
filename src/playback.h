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

    /**
     * @brief media
     * @return
     */
    inline Media* media() const {return _media;}

    /**
     * @brief mediaSettings
     * @return
     */
    inline MediaSettings* mediaSettings() const {return _mediaSettings;}
    
private:

    /**
     * @brief _media
     */
    Media *_media;

    /**
     * @brief _mediaSettings
     */
    MediaSettings *_mediaSettings;
    
};

#endif // PLAYBACK_H
