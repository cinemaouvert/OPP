#ifndef MEDIALISTMODEL_H
#define MEDIALISTMODEL_H

#include <QAbstractTableModel>

class Media;

class MediaListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Columns { Name = 0, Length = 1, Used = 2, Location = 3 };

    MediaListModel(QList<Media*> &mediaList, QObject *parent = 0);

    int columnCount(const QModelIndex &parent) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QVariant data(const QModelIndex &index, int role) const;

    void addMedia(Media *media);

private:
    QList<Media*> &_mediaList;
};

#endif // MEDIALISTMODEL_H
