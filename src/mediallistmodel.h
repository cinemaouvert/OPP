#ifndef MEDIALLISTMODEL_H
#define MEDIALLISTMODEL_H

#include <QAbstractListModel>

class Media;

class MedialListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    MedialListModel(QList<Media*> &mediaList, QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

private:
    QList<Media*> _mediaList;
};

#endif // MEDIALLISTMODEL_H
