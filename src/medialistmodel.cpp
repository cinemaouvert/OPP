#include "medialistmodel.h"

#include <QDebug>
#include <QIcon>

#include "utils.h"
#include "media.h"

MediaListModel::MediaListModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int MediaListModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

int MediaListModel::rowCount(const QModelIndex &parent) const
{
    return _mediaList.size();
}

Qt::ItemFlags MediaListModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant MediaListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case Name:
            return trUtf8("Name");
            break;
        case Duration:
            return trUtf8("Duration");
            break;
        case Used:
            return trUtf8("Used");
            break;
        case Location:
            return trUtf8("Location");
            break;
        }
    }

    return QVariant();
}

QVariant MediaListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= _mediaList.count()) {
        return QVariant();
    }

    switch (role)
    {
    case Qt::ToolTipRole:
        switch (index.column()) {
        case Name:
            return _mediaList[index.row()].name();
            break;
        case Location:
            return _mediaList[index.row()].location();
            break;
        case Duration:
            return msecToQTime(_mediaList[index.row()].duration()).toString("hh:mm:ss");
            break;
        }
        break;
    case Qt::DecorationRole:
        if (index.column() == Used) {
            QIcon icon;
            if (_mediaList[index.row()].isUsed())
                icon.addFile(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_152_check.png"), QSize(), QIcon::Normal, QIcon::Off);
            else
                icon.addFile(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_153_unchecked.png"), QSize(), QIcon::Normal, QIcon::Off);
            return icon;
        }
        break;
    case Qt::DisplayRole:
        if (index.column() == Location) {
            return _mediaList[index.row()].location();
        }
        if (index.column() == Name) {
            return _mediaList[index.row()].name();
        }
        if (index.column() == Duration) {
            return msecToQTime(_mediaList[index.row()].duration()).toString("hh:mm:ss");
        }
        break;
    }

    return QVariant();
}

bool MediaListModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row=0; row < rows; ++row) {
        _mediaList.removeAt(position);
    }

    endRemoveRows();
    return true;
}

bool MediaListModel::addMedia(const Media &media)
{
    const int count = _mediaList.count();
    if (!_mediaList.contains(media)) {
        beginInsertRows(QModelIndex(), count, count);
        _mediaList.append(media);
        connect(&_mediaList.last(), SIGNAL(usageCountChanged()), this, SIGNAL(dataChanged()));
        endInsertRows();
        return true;
    } else {
        return false;
    }
}

QDataStream & operator << (QDataStream & out, const QList<Media> &list)
{
    for(int i=0; i<list.size(); ++i){
        QString loc(list[i].location());
        out << loc;
    }
    return out;
}

QDataStream & operator >> (QDataStream & in, QList<Media> &list)
{
    in >> list;
    return in;
}
