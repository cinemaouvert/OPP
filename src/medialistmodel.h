#ifndef MEDIALISTMODEL_H
#define MEDIALISTMODEL_H

#include <QAbstractListModel>

class Media;

class MediaListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Columns { Length = 0, Name = 1, Used = 2, Location = 3 };

    MediaListModel(QList<Media*> &mediaList, QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

    void addMedia(Media *media);

//    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

private:
    QList<Media*> &_mediaList;
};

#endif // MEDIALISTMODEL_H
