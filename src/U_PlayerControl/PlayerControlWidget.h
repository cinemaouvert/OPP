#ifndef PLAYERCONTROLWIDGET_H
#define PLAYERCONTROLWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QIcon>
#include <QShortcut>

#include "MediaPlayer.h"
#include "PlaylistPlayer.h"
#include "ForwardButton.h"
#include "RewindButton.h"
#include "SeekWidget.h"
#include "LoopButton.h"

class SeekWidget;

class PlayerControlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlayerControlWidget(PlaylistPlayer* playlistPlayer, QWidget *parent = 0);
    virtual ~PlayerControlWidget();

    QPushButton* playPauseButton(){ return _playPauseButton; }

    QPushButton* stopButton(){ return _stopButton; }
    QPushButton* previousButton(){ return _previousButton; }
    QPushButton* nextButton(){ return _nextButton; }
    QPushButton* rewindButton(){ return _rewindButton; }
    QPushButton* forwardButton(){ return _forwardButton; }

    inline int currentVolume() const { return _volumeSlider->value(); }

    QShortcut* get_playPause_shortcut( ){return _playPause_shortcut;}
    QShortcut* get_previous_shortcut() {return _previous_shortcut;}
    QShortcut* get_next_shortcut() {return _next_shortcut;}
    QShortcut* get_rewind_shortcut() {return _rewind_shortcut;}
    QShortcut* get_forward_shortcut() {return _forward_shortcut;}

protected:

    /**
     * @brief create the widgets contained by the current player control widget.
     */
    void createWidgets();

    /**
     * @brief create the connections with the playlist player
     */
    void createConnections();

    void changeEvent(QEvent* e);

public slots:

    /**
     * @brief lock or unlock all the control widgets
     */
    void disableControlWidgets(bool disable);

    /**
     * @brief lockButtonClicked
     */
    void lockButtonClicked();

private slots:

    void togglePlayPauseButton();

    void playPause();

    void playPauseKeyboard();

    void emitStop();

    void emitPause();

    void enableRewindForwardBtn();

    void disableRewindForwardBtn();

signals:
    void playState();

    void pauseState();

    void stopState();

private:
    PlaylistPlayer* _playlistPlayer;

    bool _enabledClicking;
    bool _disabledClicking;
    int _enabledProgrammingCounter;

    SeekWidget* _seekWidget;

    QWidget* _buttonBar;
    QHBoxLayout* _horizontalLayout;
    QPushButton* _lockButton;
    QPushButton* _playPauseButton;
    QPushButton* _previousButton;
    QPushButton* _nextButton;
    QPushButton* _rewindButton;
    QPushButton* _forwardButton;
    QPushButton* _stopButton;
    LoopButton* _loopButton;
    QPushButton* _goToBlackButton;
    QLabel* _labelVolumeUp;
    QSlider* _volumeSlider;
    QLabel* _volumeLabel;

    QShortcut* _playPause_shortcut;
    QShortcut* _previous_shortcut;
    QShortcut* _next_shortcut;
    QShortcut* _rewind_shortcut;
    QShortcut* _forward_shortcut;
};

#endif // PLAYERCONTROLWIDGET_H
