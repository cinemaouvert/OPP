#ifndef MEDIA_H
#define MEDIA_H

#include <QtCore/QObject>
#include <QFileInfo>

class Media : public QObject
{
    Q_OBJECT
public:
    Media(const QString &location, QObject *parent = 0);
    Media(const Media &media);

    Media & operator=(const Media &media);

    inline const QString & location() const { return _location; }

    QString name() const;

    bool exists() const;

    bool operator==(const Media &media) const;
private:
    QString _location;
    QFileInfo _fileInfo;
};

#endif // MEDIA_H
