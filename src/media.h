#ifndef MEDIA_H
#define MEDIA_H

#include <QtCore/QObject>
#include <QFileInfo>

class Media : public QObject
{
    Q_OBJECT
public:
    explicit Media(const QString &location, QObject *parent = 0);
    
    inline const QString & location() const { return _location; }

    QString name() const;

    bool isExists() const;

private:
    QString _location;
    QFileInfo _fileInfo;
};

#endif // MEDIA_H
