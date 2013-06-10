#include "statuswidget.h"
#include "ui_statuswidget.h"

#include <QDateTime>

StatusWidget::StatusWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusWidget)
{
    ui->setupUi(this);

    _timerId = startTimer(900);
    setMediaCount(0);

    QIcon icon;

    icon = QIcon(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_054_clock.png"));
    ui->timeIconLabel->setPixmap(icon.pixmap(16));

    icon = QIcon(QString::fromUtf8(":/icons/resources/glyphicons/glyphicons_045_calendar.png"));
    ui->dateIconLabel->setPixmap(icon.pixmap(16));
}

StatusWidget::~StatusWidget()
{
    killTimer(_timerId);
    delete ui;
}

void StatusWidget::setMediaCount(int count)
{
    ui->countLoadedFilesLabel->setText(QString("%1 %2")
        .arg( QString::number(count) )
        .arg( tr("files loaded") )
    );
}

void StatusWidget::timerEvent(QTimerEvent *event)
{
    QDateTime now = QDateTime::currentDateTime();

    ui->timeLabel->setText(now.time().toString("hh:mm:ss"));
    ui->dateLabel->setText(now.date().toString("dd/MM/yy"));
}

