#ifndef MEDIA_H
#define MEDIA_H

#include <QtCore/QObject>

class Media : public QObject
{
    Q_OBJECT
public:
    explicit Media(const QString &location, QObject *parent = 0);
    
    inline const QString & location() const { return _location; }
private:
    QString _location;
};

#endif // MEDIA_H
