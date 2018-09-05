#include "titlewidget.h"
#include "ui/Common/pushbutton.h"
#include "ui/TitleBar/my_menu.h"
#include "memowidget.h"
#include "ui/Common/menu.h"
#include "ui/TitleBar/titlewidget.h"

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

TitleWidget::TitleWidget(QWidget *parent)
    : QWidget(parent)
{

    this->initWidget();
    this->initForm();
    this->initConnect();
    this->Translator();
}

TitleWidget::~TitleWidget()
{

}

void TitleWidget::Translator()
{
    m_TbnTime->setToolTip(tr("当前时间"));

    m_TbnMenu->setIcon(QIcon(tr(":images/Settings.png")));
    m_TbnMenu->setToolTip(tr("菜单"));

    m_TbnMenu_max->setToolTip(tr("最大化"));
    m_TbnMenu_max->setIcon(QIcon(tr(":images/zoom.png")));
    m_TbnMenu_min->setToolTip(tr("最小化"));
    m_TbnMenu_min->setIcon(QIcon(tr(":images/minimize_3.png")));
    m_TbnMenu_close->setToolTip(tr("关闭"));
    m_TbnMenu_close->setIcon(QIcon(tr(":images/close.png")));
}

void TitleWidget::slotChangeBtnStatus(int status)
{
    switch (status) {
    case BtnMax:
        m_TbnMenu_max->setToolTip(tr("最大化"));
        break;
    case BtnMin:
        m_TbnMenu_min->setToolTip(tr("恢复"));
        break;
    default:
        break;
    }
}

//界面
void TitleWidget::initForm()
{
    this->setMaximumHeight(40);
    // this->setMinimumWidth(630);
    //click to focus.
    this->setFocusPolicy(Qt::ClickFocus);


    m_TbnMenu->setObjectName("pbnMenu");
    m_TbnMenu_max->setObjectName("pbnMax");
    m_TbnMenu_min->setObjectName("pbnMin");
    m_TbnMenu_close->setObjectName("pbnClose");
    m_TbnTime->setObjectName("pbnTime");
}

//初始化组件、布局
void TitleWidget::initWidget()
{
    m_TbnTime = new QLabel(this);

    m_TbnTime->setFixedSize(80, 20);
    m_TbnTime->setStyleSheet("color:#646464; border: 1px solid gray; border-radius:2px; text-align:center;");
    m_TbnTime->setAlignment(Qt::AlignCenter);
    m_TbnTime->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
    m_Timer = new QTimer(this);
    m_TimerPlay = new QTimer(this);
    connect(m_Timer, &QTimer::timeout,
            this, &TitleWidget::slotUpdateTime);
    m_Timer->start(1000);
    m_TbnTime->show();
    m_TimerPlay->start(1000);
    connect(m_TimerPlay, &QTimer::timeout,
            this, &TitleWidget::slotPrePlay);
    connect(this, &TitleWidget::signalPlay,
            this, &TitleWidget::slotPlay);

    everydayEnglish = new QLabel(this);
    everydayEnglish->setText(tr("Everyday English."));

    m_TbnMenu = new QPushButton(this);
    m_TbnMenu->setStyleSheet("color:#226DDD;");
    m_TbnMenu_min = new QPushButton(this);
    m_TbnMenu_max = new QPushButton(this);
    m_TbnMenu_close = new QPushButton(this);

    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->addSpacing(15);
    m_mainLayout->addWidget(m_TbnTime);
    m_mainLayout->addSpacing(20);
    m_mainLayout->addWidget(everydayEnglish);

    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_TbnMenu);
    m_mainLayout->addSpacing(10);
    m_mainLayout->addWidget(m_TbnMenu_min);
    m_mainLayout->addWidget(m_TbnMenu_max);
    m_mainLayout->addWidget(m_TbnMenu_close);
    m_mainLayout->addSpacing(6);

    this->setLayout(m_mainLayout);
    m_menu = new MyMenu(this);
    m_TbnMenu->setMenu(m_menu);

    player = new QMediaPlayer(this);
    playList = new QMediaPlaylist(this);
    player->setPlaybackRate(1.0);
    player->setVolume(50);
    player->setPlaylist(playList);

}

//信号槽连接
void TitleWidget::initConnect()
{
    connect(m_TbnMenu_max, &QPushButton::clicked,
            this, &TitleWidget::signalMax);
    connect(m_TbnMenu_min, &QPushButton::clicked,
            this, &TitleWidget::signalMin);
    connect(m_TbnMenu_close, &QPushButton::clicked,
            this, &TitleWidget::signalClose);
}

void TitleWidget::slotSetEveryDayEnglish(const QString &parse)
{
    everydayEnglish->setText(parse);
}

void TitleWidget::slotUpdateTime()
{
    m_TbnTime->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
}

void TitleWidget::slotPrePlay()
{
    int times = 1;
    bool flag = false;
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    // to practise the regex
    QRegularExpression re;
    re.setPattern("^(\\d\\d):(\\d\\d):(\\d\\d)$");
    QRegularExpressionMatch match = re.match(time);
    qint32 hour = 0, minute = 0, second = 0;
    if (match.hasMatch()) {
        hour = match.captured(1).toInt();
        minute = match.captured(2).toInt();
        second = match.captured(3).toInt();
    }
    // qDebug() << hour << " " << minute << " " << second;

    if (hour > 12)
        hour -= 12;
    if (minute % 30 == 0)
        flag = true;
    else
        flag = false;

    if (minute == 0)
        times = hour;
    else
        times = 1;

    // like a event filter.
    if (flag && second == 0)
        emit signalPlay(times);
}

void TitleWidget::slotPlay(int times)
{
    playList->clear();
    for (int i = 0; i < times; i++)
        playList->addMedia(QUrl("qrc:sounds/tictoe.mp3"));
    playList->setCurrentIndex(0);

    player->play();
}
