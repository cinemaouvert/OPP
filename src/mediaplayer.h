#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>

class MediaPlayer : public QObject
{
    Q_OBJECT
public:
    explicit MediaPlayer(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // MEDIAPLAYER_H
