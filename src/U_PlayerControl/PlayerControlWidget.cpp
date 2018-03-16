
#include <QShortcut>

#include "mainwindow.h"
#include "PlayerControlWidget.h"
#include "LoopButton.h"

PlayerControlWidget::PlayerControlWidget(PlaylistPlayer* playlistPlayer, QWidget *parent) :
    QWidget(parent),
    _enabledClicking(false),
    _disabledClicking(false),
    _enabledProgrammingCounter(0)
{
    _playlistPlayer = playlistPlayer;

    setLayout(new QVBoxLayout);

    setStyleSheet(QString(
        "QPushButton{"
        #if defined(Q_OS_MAC)
        "   margin: 0px 10px 0px 0px;"
        #else
        "   margin: 0px 0px 0px 0px;"
        #endif
        "   padding: 5px 0px 5px 0px;"
        "   border-radius: 0px;"
        "   border: 1px solid rgb(190, 190, 190);"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 rgb(240,240,240), stop:1 rgb(223,223,223));"
        "}"
        "QPushButton:!enabled{"
        "   border: 1px solid rgb(210, 210, 210);"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 rgb(255,255,255), stop:1 rgb(238,238,238));"
        "}"
        "QPushButton:pressed{"
        "   border: 1px solid rgb(153, 153, 153);"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 rgb(220,220,220), stop:1 rgb(203,203,203));"
        "}"
        "QPushButton:selected{"
        "   border: 1px solid rgb(153, 153, 153);"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 rgb(220,220,220), stop:1 rgb(203,203,203));"
        "}"
    ));

    _seekWidget = new SeekWidget(this);
    _seekWidget->setPlaylistPlayer(_playlistPlayer);

    createWidgets();
    createConnections();

    layout()->setContentsMargins(QMargins(0, 0, 0, 10));
    layout()->setSpacing(0);
    layout()->addWidget(_seekWidget);
    layout()->addWidget(_buttonBar);

    connect(_seekWidget->flipBar(), SIGNAL(positionManuallyChanged()), this, SLOT(emitPause()));
    connect(_seekWidget->flipBar(), SIGNAL(positionManuallyChanged()), this, SLOT(enableRewindForwardBtn()));

    // Shortcuts
    /*
    QShortcut* playPause_shortcut = new QShortcut(QKeySequence("space"), this);
    QShortcut* previous_shortcut = new QShortcut(QKeySequence("up"), this);
    QShortcut* next_shortcut = new QShortcut(QKeySequence("down"), this);
    QShortcut* rewind_shortcut = new QShortcut(QKeySequence("left"), this);
    QShortcut* forward_shortcut = new QShortcut(QKeySequence("right"), this);
    QShortcut* stop_shortcut = new QShortcut(QKeySequence("s"), this);
    QShortcut* lock_shortcut = new QShortcut(QKeySequence("l"), this);
    QShortcut* loop_shortcut = new QShortcut(QKeySequence("p"), this);

    connect(playPause_shortcut, SIGNAL(activated()), this, SLOT(playPauseKeyboard()));
    connect(previous_shortcut, SIGNAL(activated()), _previousButton, SLOT(click()));
    connect(next_shortcut, SIGNAL(activated()), _nextButton, SLOT(click()));
    connect(rewind_shortcut, SIGNAL(activated()), _rewindButton, SLOT(click()));
    connect(forward_shortcut, SIGNAL(activated()), _forwardButton, SLOT(click()));
    connect(stop_shortcut, SIGNAL(activated()), _stopButton, SLOT(click()));
    connect(lock_shortcut, SIGNAL(activated()), _lockButton, SLOT(click()));
    connect(loop_shortcut, SIGNAL(activated()), _loopButton, SLOT(click()));
    */
}

PlayerControlWidget::~PlayerControlWidget()
{
    delete _seekWidget;
    delete _buttonBar;
}

void PlayerControlWidget::createWidgets()
{
    // HORIZONTAL LAYOUT
    _horizontalLayout = new QHBoxLayout;
    _horizontalLayout->setContentsMargins(0, 0, 0, 0);
    _horizontalLayout->setSpacing(4);

    // LOCK BUTTON
    _lockButton = new QPushButton(this);
    _lockButton->setMinimumSize(QSize(50, 0));
    QIcon icon7;
    icon7.addFile(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_204_unlock.png"), QSize(), QIcon::Normal, QIcon::Off);
    icon7.addFile(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_203_lock.png"), QSize(), QIcon::Normal, QIcon::On);
    _lockButton->setIcon(icon7);
    _lockButton->setCheckable(true);
    _lockButton->setChecked(false);
    _lockButton->setFocusPolicy(Qt::NoFocus);
    _horizontalLayout->addWidget(_lockButton);

    // PLAY PAUSE BUTTON
    _playPauseButton = new QPushButton(this);
    _playPauseButton->setMinimumSize(QSize(50, 0));
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_173_play.png"), QSize(), QIcon::Normal, QIcon::Off);
    icon1.addFile(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_174_pause.png"), QSize(), QIcon::Normal, QIcon::On);
    _playPauseButton->setIcon(icon1);
    _playPauseButton->setCheckable(true);
    _playPauseButton->setChecked(false);
    _playPauseButton->setFocusPolicy(Qt::NoFocus);

    _horizontalLayout->addWidget(_playPauseButton);

    // PREVIOUS BUTTON
    _previousButton = new QPushButton(this);
    _previousButton->setMinimumSize(QSize(50, 0));
    QIcon icon2;
    icon2.addFile(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_170_step_backward.png"), QSize(), QIcon::Normal, QIcon::Off);
    _previousButton->setIcon(icon2);
    _previousButton->setFocusPolicy(Qt::NoFocus);

    _horizontalLayout->addWidget(_previousButton);

    // NEXT BUTTON
    _nextButton = new QPushButton(this);
    _nextButton->setMinimumSize(QSize(50, 0));
    QIcon icon3;
    icon3.addFile(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_178_step_forward.png"), QSize(), QIcon::Normal, QIcon::Off);
    _nextButton->setIcon(icon3);
    _nextButton->setFocusPolicy(Qt::NoFocus);

    _horizontalLayout->addWidget(_nextButton);

    // REWIND BUTTON
    _rewindButton = new RewindButton(_playlistPlayer, this);
    _rewindButton->setFocusPolicy(Qt::NoFocus);

    _horizontalLayout->addWidget(_rewindButton);

    // FORWARD BUTTON
    _forwardButton = new ForwardButton(_playlistPlayer, this);
    _forwardButton->setFocusPolicy(Qt::NoFocus);

    _horizontalLayout->addWidget(_forwardButton);

    // STOP BUTTON
    _stopButton = new QPushButton(this);
    _stopButton->setMinimumSize(QSize(50, 0));
    QIcon icon4;
    icon4.addFile(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_175_stop.png"), QSize(), QIcon::Normal, QIcon::Off);
    _stopButton->setIcon(icon4);
    _stopButton->setFocusPolicy(Qt::NoFocus);

    _horizontalLayout->addWidget(_stopButton);

    // LOOP BUTTON
    _loopButton = new LoopButton(_playlistPlayer, this);
    _loopButton->setFocusPolicy(Qt::NoFocus);

    _horizontalLayout->addWidget(_loopButton);

    // GO TO BLACK BUTTON
    _goToBlackButton = new QPushButton(this);
    _goToBlackButton->setMinimumSize(QSize(50, 0));
    QIcon icon5;
    icon5.addFile(QString::fromUtf8(":/icons/resources/glyphicons/goToBlack.png"), QSize(), QIcon::Normal, QIcon::Off);
    _goToBlackButton->setIcon(icon5);
    _goToBlackButton->setToolTip("Go To Black");
    _goToBlackButton->setFocusPolicy(Qt::NoFocus);

    _horizontalLayout->addWidget(_goToBlackButton);

    // LABEL VOLUME UP
    _labelVolumeUp = new QLabel(this);
    _labelVolumeUp->setPixmap(QPixmap(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_184_volume_up.png"))
                              .scaled(18, 18, Qt::KeepAspectRatio, Qt::SmoothTransformation)
                              );
    _labelVolumeUp->setAlignment(Qt::AlignCenter);
    _labelVolumeUp->setContentsMargins(QMargins(7, 2, 0, 0));
    _horizontalLayout->addWidget(_labelVolumeUp);

    // VOLUME SLIDER
    _volumeSlider = new QSlider(this);
    _volumeSlider->setMaximumSize(QSize(500, 16777215));
    _volumeSlider->setMaximum(100);
    _volumeSlider->setValue(100);
    _volumeSlider->setTracking(true);
    _volumeSlider->setOrientation(Qt::Horizontal);
    _volumeSlider->setTickPosition(QSlider::TicksBelow);
    _volumeSlider->setTickInterval(12.5);

    _volumeSlider->setStyleSheet(QString(
        "QSlider::groove:horizontal {"
        "   border-bottom: 1px solid rgb(250, 250, 250);"
        "   height: 2px;"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(220, 220, 220), stop:1 rgb(170, 170, 170));"
        "   margin: 2px 0;"
        "}"
        "QSlider::groove:horizontal:!enabled {"
        "   border-bottom: 1px solid rgb(255, 255, 255);"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(240, 240, 240), stop:1 rgb(190, 190, 190));"
        "}"
        "QSlider::sub-page:horizontal {"
        "   border-top: 1px solid rgb(130, 130, 130);"
        "   height: 2px;"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(190, 190, 190), stop:1 rgb(170, 170, 170));"
        "   margin: 2px 0;"
        "}"
        "QSlider::sub-page:horizontal:!enabled {"
        "   border-top: 1px solid rgb(140, 140, 140);"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(200, 200, 200), stop:1 rgb(180, 180, 180));"
        "}"
        "QSlider::handle:horizontal {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(90, 90, 90), stop:1 rgb(50, 50, 50));"
        "   width: 6px;"
        "   margin: -7px 0;"
        "}"
        "QSlider::handle:horizontal:!enabled {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(110, 110, 110), stop:1 rgb(70, 70, 70));"
        "}"
    ));

    _horizontalLayout->addWidget(_volumeSlider);

    // LABEL VOLUME LEVEL
    _volumeLabel = new QLabel(this);
    _volumeLabel->setNum(100);
    _volumeLabel->setStyleSheet("font-size: 13px;");
    _volumeLabel->setAlignment(Qt::AlignCenter);
    _volumeLabel->setContentsMargins(QMargins(0,0,0,0));

    _horizontalLayout->addWidget(_volumeLabel);

    // BUTTON BAR
    _buttonBar = new QWidget(this);
    _buttonBar->setLayout(_horizontalLayout);
    _horizontalLayout->setSpacing(4);
}

void PlayerControlWidget::createConnections()
{
    MediaPlayer* mediaPlayer = _playlistPlayer->mediaPlayer();
    mediaPlayer->setVolume(_volumeSlider->value());

    connect(_playPauseButton, SIGNAL(clicked()), this, SLOT(playPause()));
    connect(_volumeSlider, SIGNAL(valueChanged(int)), mediaPlayer, SLOT(setVolume(int)));
    connect(_volumeSlider, SIGNAL(valueChanged(int)), _volumeLabel, SLOT(setNum(int)));
    connect(_previousButton, SIGNAL(clicked()), _playlistPlayer, SLOT(previous()));
    connect(_nextButton, SIGNAL(clicked()), _playlistPlayer, SLOT(next()));
    connect(_rewindButton , SIGNAL(pressed()), _playlistPlayer, SLOT(rewind()));
    connect(_forwardButton, SIGNAL(pressed()), _playlistPlayer, SLOT(forward()));
    connect(_stopButton, SIGNAL(clicked()), _playlistPlayer, SLOT(stop()));
    connect(_stopButton, SIGNAL(clicked()), this, SLOT(emitStop()));

    // Disable the forward and rewind buttons when the stop is declared enable otherwise
    connect(_stopButton, SIGNAL(clicked()), this, SLOT(disableRewindForwardBtn()));
    connect(_playPauseButton, SIGNAL(clicked()), this, SLOT(enableRewindForwardBtn()));

    connect(mediaPlayer, SIGNAL(stopped()), this, SLOT(togglePlayPauseButton()));
    connect(mediaPlayer, SIGNAL(end()), this, SLOT(togglePlayPauseButton()));

    // Connect the lock button
    connect(_lockButton, SIGNAL(toggled(bool)), this, SLOT(disableControlWidgets(bool)));
    connect(_lockButton, SIGNAL(clicked()), this, SLOT(lockButtonClicked()));

    // Go to black
    connect(_goToBlackButton, SIGNAL(clicked()), mediaPlayer, SLOT(goToBlack()));
    connect(mediaPlayer, SIGNAL(endGoToBlack()), this, SLOT(emitStop()));

    // Video Window closed
    connect(((MainWindow*)window())->videoWindow(), SIGNAL(closed()), this, SLOT(emitStop()));
}

void PlayerControlWidget::togglePlayPauseButton(){
    MediaPlayer* mediaPlayer = _playlistPlayer->mediaPlayer();

    if(mediaPlayer->isPlaying()){
        if(!_playPauseButton->isChecked())
            _playPauseButton->setChecked(true);
    }else{
        if(_playPauseButton->isChecked())
            _playPauseButton->setChecked(false);
    }
}

void PlayerControlWidget::playPause(){
    if(_playPauseButton->isEnabled() && isEnabled()){
        if (_playlistPlayer->mediaPlayer()->isStopped() || _playlistPlayer->mediaPlayer()->isPaused()) {
            _playlistPlayer->play();

            if(!_playPauseButton->isChecked())
                _playPauseButton->setChecked(true);

            emit playState();  // Must be after, Important !
        } else {
            _playlistPlayer->mediaPlayer()->pause();
            if(_playPauseButton->isChecked())
                _playPauseButton->setChecked(false);

            emit pauseState(); // Must be after, Important !
        }
    }
}

void PlayerControlWidget::playPauseKeyboard()
{
    if(_playPauseButton->isEnabled() && isEnabled()){
        if (_playlistPlayer->mediaPlayer()->isStopped() || _playlistPlayer->mediaPlayer()->isPaused()) {
            _playlistPlayer->play();

            if(!_playPauseButton->isChecked())
                _playPauseButton->setChecked(true);

            emit playState();  // Must be after, Important !
        } else {
            _playlistPlayer->mediaPlayer()->pause();
            if(_playPauseButton->isChecked())
                _playPauseButton->setChecked(false);

            emit pauseState(); // Must be after, Important !
        }
    }
}

void PlayerControlWidget::disableControlWidgets(bool disable){
    // The seek widget
    _seekWidget->setEnabled(!disable);

    // The button bar
    _lockButton->setEnabled(true);
    _playPauseButton->setEnabled(!disable);
    _previousButton->setEnabled(!disable);
    _nextButton->setEnabled(!disable);
    _rewindButton->setEnabled(!disable);
    _forwardButton->setEnabled(!disable);
    _stopButton->setEnabled(!disable);
    _loopButton->setEnabled(!disable);
    _goToBlackButton->setEnabled(!disable);
    _labelVolumeUp->setEnabled(!disable);
    _volumeSlider->setEnabled(!disable);
    _volumeLabel->setEnabled(!disable);

    if(_playlistPlayer->mediaPlayer()->isStopped())
        disableRewindForwardBtn();
    else if(!disable)
        enableRewindForwardBtn();
}

void PlayerControlWidget::lockButtonClicked(){
    if(_stopButton->isEnabled()){
        _enabledClicking = true;
        _disabledClicking = false;
    }else{
        _enabledClicking = false;
        _disabledClicking = true;
    }
}

void PlayerControlWidget::changeEvent(QEvent* e)
{
    e->ignore();

    if(e->type() == QEvent::EnabledChange){
        /**** count the number of time where the current widget is enabled by the programme ****/
        _enabledProgrammingCounter++;

        /**** the program lead the lock action ****/
        if(_enabledProgrammingCounter < 2){
            _lockButton->setChecked(!isEnabled());
            return;
        }else{
            _enabledProgrammingCounter = 0;
        }

        /**** the lock action is lead by the user decision ****/
        if(_enabledClicking && !isEnabled()){
            _lockButton->setChecked(false);
            return;
        }

        if(_disabledClicking && isEnabled()){
            _lockButton->setChecked(true);
            return;
        }

        /**** Default action ****/
        _lockButton->setChecked(!isEnabled());

        if(isEnabled() && _playlistPlayer->mediaPlayer()->isStopped())
            disableRewindForwardBtn();
    }
}

void PlayerControlWidget::emitStop()
{
    emit stopState();
}

void PlayerControlWidget::emitPause()
{
    MediaPlayer* mediaPlayer = _playlistPlayer->mediaPlayer();

    if(!mediaPlayer->isPaused())
        emit pauseState();
}

void PlayerControlWidget::enableRewindForwardBtn()
{
    if(!_rewindButton->isEnabled()){
        _rewindButton->setEnabled(true);
        _forwardButton->setEnabled(true);
    }
}

void PlayerControlWidget::disableRewindForwardBtn()
{
    if(_rewindButton->isEnabled()){
        _rewindButton->setEnabled(false);
        _forwardButton->setEnabled(false);
    }
}
