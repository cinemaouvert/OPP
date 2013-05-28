#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>

#include "media.h"

class MediaPlayer;

class Project : public QObject
{
    Q_OBJECT
public:
    explicit Project(QObject *parent = 0);
    virtual ~Project();

    inline QList<Media>& mediaList() { return _mediaList; }
    inline MediaPlayer* mediaPlayer() const;

    void saveToFile(const QString & location) const;

    static Project* makeFromFile(const QString & location);

signals:
    void mediaAdded(Media *media);
    void mediaRemoved(Media *media);

private:
    QList<Media> _mediaList;
    MediaPlayer *_mediaPlayer;
};

#endif // PROJECT_H
