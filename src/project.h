#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>

#include "media.h"

class Project : public QObject
{
    Q_OBJECT
public:
    explicit Project(QObject *parent = 0);
    virtual ~Project();

    /**
     * @brief mediaList
     * @return
     */
    inline QList<Media>& mediaList() { return _mediaList; }

    /**
     * @brief saveToFile
     * @param location
     */
    void saveToFile(const QString & location) const;

    /**
     * @brief makeFromFile
     * @param location
     * @return
     */
    static Project* makeFromFile(const QString & location);

signals:

    /**
     * @brief mediaAdded
     * @param media
     */
    void mediaAdded(Media *media);

    /**
     * @brief mediaRemoved
     * @param media
     */
    void mediaRemoved(Media *media);

private:

    /**
     * @brief _mediaList
     */
    QList<Media> _mediaList;
};

#endif // PROJECT_H
