#ifndef PLAYLISTLISTMANAGER_H
#define PLAYLISTLISTMANAGER_H

#include <QObject>

class PlaylistListManager : public QObject
{
    Q_OBJECT
public:
    explicit PlaylistListManager(QObject *parent = 0);
    
//    void addItem(PlaylistModel *model);
//    void removeItem(int index);
//    inline const QList<PlaylistModel*> &items();
//    void setCurrentIndex(int index);
//    void clear();
private:

//    QTabWidget *viewList;
    
};

#endif // PLAYLISTLISTMANAGER_H
