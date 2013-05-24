#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>

class Media;

class Project : public QObject
{
    Q_OBJECT
public:
    explicit Project(QObject *parent = 0);
    
    void saveToFile(const QString & location) const;

    void addMedia(Media *);

    void removeMedia(Media *);

    inline QList<Media*>& mediaList() const;

    static Project* makeFromFile(const QString & location);
signals:
    
public slots:
    

private:

    QList<Media*> _mediaList;
};

#endif // PROJECT_H
