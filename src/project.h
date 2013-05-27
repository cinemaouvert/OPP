#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>

class Media;

class Project : public QObject
{
    Q_OBJECT
public:
    explicit Project(QObject *parent = 0);

    inline QList<Media*>& mediaList() { return _mediaList; }

    void saveToFile(const QString & location) const;

    bool addMedia(Media *media);

    static Project* makeFromFile(const QString & location);

signals:
    mediaAdded(Media *media);
    mediaRemoved(Media *media);

private:
    QList<Media*> _mediaList;
};

#endif // PROJECT_H
