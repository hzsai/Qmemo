#include "memowidget.h"
#include "ui/TitleBar/titlewidget.h"
#include "ui/MidLeftWidget/datewidget.h"
#include "ui/MidRightDownWidget/rightdownwidget.h"
#include "ui/MidRightUpWidget/rightupwidget.h"
#include "ui/TitleBar/my_menu.h"
#include "database/database.h"
#include "plugins/qreplytimeout.h"

#include <iostream>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QApplication>
#include <QAbstractButton>
#include <QPushButton>
#include <QBitmap>
#include <QPainter>
#include <QDebug>
#include <QLabel>
#include <QSettings>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>

bool g_WidgetIsMin = false;

MemoWidget::MemoWidget(QWidget *parent)
    :QFrame(parent)
{
    database = new DataBase(this);
    this->initBaseInfo();
    this->initWidget();
    this->initForm();
    this->initConnect();
}

MemoWidget::~MemoWidget()
{

}

void MemoWidget::initBaseInfo()
{
    icibaword = new IcibaWord();
    qnam = new QNetworkAccessManager(this);
}

void MemoWidget::initForm()
{
    //初始化界面大小、有无边框
    //this->setMinimumSize(850, 650);
    this->setFixedSize(850, 650);
    this->setWindowFlags(Qt::FramelessWindowHint |
                         Qt::WindowSystemMenuHint |
                         Qt::WindowMinimizeButtonHint);
    this->setWindowTitle(QString::fromLocal8Bit("记事本"));
    this->setObjectName("MainFrame");
    m_location = this->geometry();
    m_isMax = false;
    m_isPressed = false;
}

//初始化界面，在frame上加组件，布局
void MemoWidget::initWidget()
{

    //顶部
    m_title = new TitleWidget(this);

    m_title->installEventFilter(this);

    //右
    m_rightUpWidget = new RightUpWidget(this);
    m_rightDownWidget = new RightDownWidget(this);

    //布局右
    m_rightLayout = new QVBoxLayout();
    m_rightLayout->addWidget(m_rightUpWidget);
    m_rightLayout->addWidget(m_rightDownWidget);

    m_leftDateWidget = new DateWidget(this);

    //总中间
    m_midLayout = new QHBoxLayout();
    m_midLayout->addWidget(m_leftDateWidget);		//日期
    m_midLayout->addLayout(m_rightLayout);			//右边组件
    m_midLayout->setMargin(0);
    m_midLayout->setContentsMargins(0, 0, 0, 0);

    //总布局
    m_maintLayout = new QVBoxLayout(this);
    m_maintLayout->addWidget(m_title);
    m_maintLayout->addLayout(m_midLayout);
    m_maintLayout->setContentsMargins(9, 2, 2, 2);
    this->setLayout(m_maintLayout);

}

void MemoWidget::initConnect()
{
    //标题栏
    connect(m_title, &TitleWidget::signalClose,
            this, &MemoWidget::slotClose);
    connect(m_title, &TitleWidget::signalMax,
            this, &MemoWidget::slotShowMax);
    connect(m_title, &TitleWidget::signalMin,
            this, &MemoWidget::slotShowMin);
    connect(m_title->m_menu->m_actionOpenPic, &QAction::triggered,
            m_rightUpWidget, &RightUpWidget::slotSetPicPath);
    connect(m_title->m_menu->m_actionQuit, &QAction::triggered,
            this, &MemoWidget::slotClose);
    connect(m_title->m_menu->m_actionSettingFont, &QAction::triggered,
            m_rightDownWidget, &RightDownWidget::slotChoiceFont);
    //connect();
    // 每个日期格子的signal & slot
    for (int i = 0; i < 42; i++) {
        // data transfer
        connect(m_leftDateWidget->calendar->labelDay[i], &DayLabel::signalMemo,
            m_rightDownWidget, &RightDownWidget::setTextEdit);
        // date transfer
        connect(m_leftDateWidget->calendar->labelDay[i], &DayLabel::signalCurrDate,
            m_rightDownWidget, &RightDownWidget::setCurrDate);

        // refresh
        connect(m_leftDateWidget->calendar->labelDay[i], &DayLabel::signalCurrDate,
            m_leftDateWidget->calendar, &CalendarWidget::initDate);
        // setDay
        connect(m_leftDateWidget->calendar->labelDay[i], &DayLabel::signalCurrDay,
                m_leftDateWidget->calendar, &CalendarWidget::setDay);
        connect(m_leftDateWidget->calendar->labelDay[i], &DayLabel::signalSetRedBox,
                m_leftDateWidget->calendar, &CalendarWidget::slotSetRedBox);
    }
    // refresh LabelIcon
    // 相应日期的变化
    connect(m_rightDownWidget, &RightDownWidget::signalSetIcon,
           m_leftDateWidget->calendar,&CalendarWidget::setLabelIcon);
    // refresh
    // 刷新整个日历组件数据signal
    connect(m_rightDownWidget->m_save, &QPushButton::clicked,
            m_leftDateWidget->calendar, &CalendarWidget::initDate);
    // store back
    connect(m_rightDownWidget, &RightDownWidget::signalRightDownWidgetSave,
            m_leftDateWidget->calendar, &CalendarWidget::slotRefreshMemo);

    // 发起请求关联信号
    connect(m_title->m_menu->getEveryDayPicture, &QAction::triggered,
            this, &MemoWidget::slotDealWithRequest);
    void (MemoWidget:: *signaldealwithfiletype)(QString, int) = &MemoWidget::signalDealWithJsonTypeFinished;
    void (MemoWidget:: *slotcreaterequest)(QString, int) = &MemoWidget::createRequest;
    connect(this, signaldealwithfiletype, this, slotcreaterequest);

    void (MemoWidget:: *signaldealwithfilefinished)(QString) = &MemoWidget::signalDealWithFileTypeFinished;
    void (RightUpWidget:: *slotloadimage) (QString) = &RightUpWidget::slotLoadImage;
    connect(this, signaldealwithfilefinished, m_rightUpWidget, slotloadimage);

    connect(this, &MemoWidget::signalParseJsonError,
            m_leftDateWidget, &DateWidget::sltDisplaymsg);
    connect(this, &MemoWidget::signalCommonMsg,
            m_leftDateWidget, &DateWidget::sltDisplaymsg);
}

void MemoWidget::slotClose()
{
    if(m_rightDownWidget->maybeSave()) {
        return ;
    }
    else {
        qApp->exit();
    }
}

//最大化
void MemoWidget::slotShowMax()
{
    if (m_isMax) {
        this->setGeometry(m_location);
        emit signalMax(BtnMax);
    } else {
        m_location = this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
        emit signalMax(BtnMin);
    }

    g_WidgetIsMin = false;
    m_isMax = !m_isMax;
}

//最小化
void MemoWidget::slotShowMin()
{
    g_WidgetIsMin = true;
    this->showMinimized();
}

void MemoWidget::createRequest()
{
    if (qnam == nullptr)
        return ;
}

// 这里写的有点问题，暂时不解决
// 在这里，测试超时连接的相关,将slots绑到MidLeftWidget的labelinfo上
// --fix
void MemoWidget::createRequest(QString url)
{
    if (qnam == nullptr)
        return ;
    reply = qnam->get(QNetworkRequest(QUrl(url)));
    QReplyTimeout *replyTimeout = new QReplyTimeout(reply, 1500);
    connect(replyTimeout, &QReplyTimeout::timeout, [=](){
        qDebug() << "createRequest, Timeout";
    });
    connect(replyTimeout, &QReplyTimeout::timeout,
            m_leftDateWidget, &DateWidget::sltDisplaymsg);
}

void MemoWidget::createRequest(QString url, int type)
{
    if (qnam == nullptr)
        return ;
    qDebug() << "URL: " << url;
    qDebug() << "Type: " << type;
    reply = qnam->get(QNetworkRequest(QUrl(url)));
    QReplyTimeout *replyTimeout = new QReplyTimeout(reply, 1500);
    connect(replyTimeout, &QReplyTimeout::timeout,
            m_leftDateWidget, &DateWidget::sltDisplaymsg);
    connect(replyTimeout, &QReplyTimeout::timeout, [=](){
        qDebug() << "createRequest with params, Timeout";
    });
    qDebug("createRequest");
    switch (type) {
    case REQ_FILE:
        connect(reply, &QNetworkReply::finished, this, &MemoWidget::slotDealWithFileType);
        break;
    case REQ_JSON:
        connect(reply, &QNetworkReply::finished, this, &MemoWidget::slotDealWithJsonType);
        break;
    case REQ_PIC:
        connect(reply, &QNetworkReply::finished, this, &MemoWidget::slotDealWithPicType);
        break;
    case REQ_TEXT:
        connect(reply, &QNetworkReply::finished, this, &MemoWidget::slotDealWithTextType);
        break;
    default:
        break;
    }
}

void MemoWidget::slotDealWithRequest()
{
    // 开始请求
    QString mtime = QDateTime::currentDateTime().addDays(0).toString("yyyy-MM-dd");
    QString url_1 = tr("http://open.iciba.com/dsapi/?date=%1").arg(mtime);

    qDebug() << "slotDealWithRequest, " << url_1;
    createRequest(url_1, REQ_JSON);
    // createRequest("https://www.google.com");
}

void MemoWidget::slotDealWithFileType()
{
    if (reply == nullptr)
        return ;
    if (reply->error() != QNetworkReply::NoError) {
        emit signalCommonMsg("网络错误！: "+reply->error());
        qDebug() << "slotDealWithJsonType, " << "error: "
                 << reply->error();
        return ;
    }
    //保存有后缀类型的文件
    // 从icibaword中获取url地址解析
    QString file_name = icibaword->parsefilename();
    qDebug() << file_name;
    QString prefix = "";
#ifdef QT_NO_DEBUG
    if (file_name.endsWith(".jpg") || file_name.endsWith(".png")) {
        prefix += "images/";
    }
#endif
    file_name = prefix + file_name;
    QFile out_file(file_name);
    qDebug() << out_file.fileName();

    if (!out_file.open(QIODevice::WriteOnly)) {
        qWarning("slotDealWithFileType, file open failed.");
        return ;
    }
    // 再读取reply的数据
    QByteArray bytearray = reply->readAll();
    out_file.write(bytearray);
    qDebug("slotDealWithFileType, 获取网络FileType数据成功!");
    emit signalDealWithFileTypeFinished(file_name);
}

void MemoWidget::slotDealWithJsonType()
{
    if (reply == nullptr)
        return ;
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "slotDealWithJsonType, " << "error: "
                 << reply->error();
        disconnect(reply, &QNetworkReply::finished, this, &MemoWidget::slotDealWithJsonType);
        return ;
    }
    // 将获得的json文件写入磁盘，并读到对象icibaword中待用
    QByteArray bytearray = reply->readAll();
    QFile out_file("./today_weather.json");
#ifndef QT_NO_DEBUG
    QFile out_temp_file("./today_weather_temp.tmp");
    if (!out_temp_file.open(QIODevice::WriteOnly)) {
        qWarning("getting, file open failed.");
        return ;
    }
    out_temp_file.write(bytearray);
    out_temp_file.close();
#endif

    if (!out_file.open(QIODevice::WriteOnly)) {
        qWarning("getting, file open failed.");
        return ;
    }
    QJsonDocument jsdoc(QJsonDocument::fromJson(bytearray));
    out_file.write(jsdoc.toJson());
    out_file.close();
    if (jsdoc.isEmpty()) {
        qDebug() << "slotDealWithJsonType, json parse error. Request type is not json type.";
        emit signalParseJsonError(QStringLiteral("解析Json异常!"));
        return ;
    }
    icibaword->read(jsdoc.object());
    emit signalCommonMsg("获取Json数据成功!");
    qDebug("slotDealWithJsonType, 获取网络JsonType数据成功!");
    emit signalDealWithJsonTypeFinished(icibaword->get_picture2(), REQ_FILE);
}

void MemoWidget::slotDealWithPicType()
{
    // 重复了,待用
    return ;
}

void MemoWidget::slotDealWithTextType()
{
    // 重复了,待用
    return ;
}

bool MemoWidget::eventFilter(QObject *obj, QEvent *event)
{
    //捕获
    if (event->type() == QEvent::MouseButtonDblClick) {
        // 将dbc事件ignora
        this->slotShowMax();
        return true;
    }
    //不符合则转发
    return QObject::eventFilter(obj, event);
}

//鼠标事件
void MemoWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isPressed && event->buttons() && Qt::LeftButton && !m_isMax) {
        this->move(event->globalPos() - m_point);
        event->accept();
    }
    qDebug() << "MemoWidget::mouseMoveEvent, ";
}

void MemoWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_point = event->globalPos() - this->pos();
        m_isPressed = true;
        event->accept();
    }
}

void MemoWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // ignore the doubleClick event
        //slotShowMax();
    }
}

void MemoWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isPressed = false;
    }
}

//重绘 yuanjiao
void MemoWidget::paintEvent(QPaintEvent *)
{
    //gen
    QBitmap objBitmap(size());

    QPainter painter(&objBitmap);

    painter.fillRect(rect(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(this->rect(), 15, 15);
    setMask(objBitmap);
}
