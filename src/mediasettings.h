#ifndef MEDIASETTINGS_H
#define MEDIASETTINGS_H

#include <QObject>

class MediaSettings : public QObject
{
    Q_OBJECT
public:
    explicit MediaSettings(QObject *parent = 0);
    MediaSettings(MediaSettings &mediasettings);
    MediaSettings& operator =(const MediaSettings &mediasettings);
    
signals:
    
public slots:
    
};

#endif // MEDIASETTINGS_H
