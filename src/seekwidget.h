#ifndef SEEKWIDGET_H_
#define SEEKWIDGET_H_

#include <QPoint>
#include <QWidget>

class QLabel;
class QProgressBar;
class QTimer;

class MediaPlayer;

class SeekWidget : public QWidget
{
Q_OBJECT
public:
    explicit SeekWidget(QWidget *parent = 0);

    ~SeekWidget();

    bool autoHide() const { return _autoHide; }

    void setAutoHide(const bool &autoHide);

    void setMediaPlayer(MediaPlayer *player);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private slots:
    void end();
    void updateCurrentTime(const int &time);
    void updateFullTime(const int &time);

private:
    void initSeekWidget();
    void updateEvent(const QPoint &pos);

    void lock();
    void unlock();

    bool _lock;

    MediaPlayer *_vlcMediaPlayer;

    bool _autoHide;
    QProgressBar *_seek;
    QLabel *_labelElapsed;
    QLabel *_labelFull;
};

#endif // SEEKWIDGET_H_
