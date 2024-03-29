#include "titlewidget.h"
#include "ui/Common/pushbutton.h"
#include "ui/TitleBar/my_menu.h"
#include "memowidget.h"
#include "ui/Common/menu.h"
#include "ui/TitleBar/titlewidget.h"

#include <thread>
#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QEventLoop>
#include <QCoreApplication>
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

    m_TbnMenu_max->setToolTip(tr("最大 化"));
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
    m_TbnTime->show();

    // 更新时间显示
    m_Timer = new QTimer(this);
    connect(m_Timer, &QTimer::timeout,
            this, &TitleWidget::slotUpdateTime);
    m_Timer->start(1000);

    // 检测整点半点
    m_TimerPlay = new QTimer(this);
    connect(m_TimerPlay, &QTimer::timeout,
            this, &TitleWidget::slotPrePlay);
    m_TimerPlay->start(1000);

    // 播放
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
    m_mainLayout->addSpacing(0);
    m_mainLayout->addWidget(m_TbnTime);
    m_mainLayout->addSpacing(5);
    m_mainLayout->addWidget(everydayEnglish);

    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_TbnMenu);
    m_mainLayout->addSpacing(5);
    m_mainLayout->addWidget(m_TbnMenu_min);
    m_mainLayout->addWidget(m_TbnMenu_max);
    m_mainLayout->addWidget(m_TbnMenu_close);
    m_mainLayout->addSpacing(5);

    this->setLayout(m_mainLayout);
    m_menu = new MyMenu(this);
    m_TbnMenu->setMenu(m_menu);

    player = new QMediaPlayer(this);
    playList = new QMediaPlaylist(this);
    player->setPlaybackRate(1.0);
    player->setVolume(50);
    player->setPlaylist(nullptr);

    player->setMedia(QMediaContent(QUrl("qrc:sounds/tictoe.mp3")), nullptr);

    connect(player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error),
        this, [this](QMediaPlayer::Error error) {
            qDebug() << "MediaPlayerError: " << error << player->errorString();
    });

    connect(this, &TitleWidget::sig_play, this, &TitleWidget::slt_playerPlay, Qt::QueuedConnection);

    connect(player, &QMediaPlayer::stateChanged, this, [&](QMediaPlayer::State status) {
        if (status == QMediaPlayer::State::StoppedState)
        {
            if (playTimes_--)
            {
                player->play();
            }
        }
    });
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

void TitleWidget::slotSetEveryDayEnglish(const IcibaWord &word)
{
    QString txt = word.getContent() + " ( " + word.getNote() + ")";
    everydayEnglish->setText(txt);
}

void TitleWidget::slt_playerPlay()
{
    this->playTimes_--;

    player->play();
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

    if (hour > 12)
        hour -= 12;
    if (minute % 30 == 0)
        flag = true;
    else
        flag = false;
#ifdef TEST_PLAYER_ALERT
    flag = true;
#endif

    if (minute == 0)
        times = hour;
    else
        times = 1;
    // 00:00:00
    if (hour == 0 && minute == 0 && second == 0)
    {
        times = 12;
    }

    // 整点、半点报时
    if (flag && second == 0)
    {
        player->stop();
        emit signalPlay(times);
    }
}

void TitleWidget::slotPlay(int times)
{
    if (times <= 0)
        return;

    this->playTimes_ = times;

    emit sig_play();

    // 使用QMediaPlaylist播放时，player->play()就crash了，暂没找到原因
	//QMediaPlaylist* nplayList = new QMediaPlaylist(this);

	//for (int i = 0; i < times; i++)
	//{
	//	bool succ = nplayList->addMedia(QMediaContent(QUrl("qrc:sounds/tictoe.mp3")));
	//	qDebug() << "Add media :" << succ;
	//}

	//player->setPlaylist(nplayList);
	//if (player->playlist() != playList)
	//{
	//	std::swap(nplayList, playList);
	//	delete nplayList;
	//}

 //   qDebug() << player->playlist()->currentIndex();
 //   if (player->isAudioAvailable())
 //   {
 //       try {
 //           player->play();
 //       }
 //       catch (std::exception e)
 //       {
 //           qDebug() << e.what();
 //       }
 //   }
}
