#include "datewidget.h"
#include "database/database.h"
#include "ui/MidLeftWidget/date.h"

#include <QInputDialog>
#include <QPainter>
#include <QStyleOption>
#include <QDateTime>
#include <QPushButton>
#include <QLineEdit>
#include <QGroupBox>
#include <QLabel>
#include <QDate>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSpacerItem>
#include <QMouseEvent>
#include <QDebug>
#include <QFont>
#include <QTimer>

static int FIRST = 1;

// 简述：
// 这是Qmemo的日历模块，有以下几部分组成：
// 1.lableTitle, 日历的标题头 2.跳转组件，有三个QLabel用于显示于QLineEdit相应的文字，有三个QLineEdit用于输入年月日，两个跳转按钮(PushButton)，跳转，回到今天
// 3.widgetBody，组织日历的组件。
// 细节上：
// 1.labelTitle是个QLabel，设置他的显示内容和样式
// 2.三个QLineEidt,三个QLabel,两个PushButton，triggered跳转时，响应slot函数，转为int，判断再调用initDate();
// 3.每一个显示一个日历的组件都是自定义的，继承自QLabel, 组件上有:labelIcon, 是否被正在选择，设置颜色，鼠标进入，离开，点击，
// 应为是竖直放置的日历，布局上是title+dateWidget的水平布局
CalendarWidget::CalendarWidget(QWidget *parent)
    :QWidget(parent)
{
    m_nYear = QDate::currentDate().year();
    m_nMonth = QDate::currentDate().month();
    m_nDay = QDate::currentDate().day();
    fixedCurrDay = QDate::currentDate().day();

    initWidget();
    initDate();
}

//你好吗，刚才什么都不能打出来额
CalendarWidget::~CalendarWidget()
{
}

static const QString s_strWeek[] = {
    QObject::tr("周日"), QObject::tr("周一"), QObject::tr("周二"),
    QObject::tr("周三"), QObject::tr("周四"), QObject::tr("周五"),
    QObject::tr("周六"),
};

int CalendarWidget::year() const
{
    return m_nYear;
}

void CalendarWidget::setYear(int year)
{
    m_nYear = year;
}

int CalendarWidget::month() const
{
    return m_nMonth;
}

void CalendarWidget::setMonth(int month)
{
    m_nMonth = month;
}

int CalendarWidget::day() const
{
    return m_nDay;
}

void CalendarWidget::setDay(int day)
{
    m_nDay = day;
    emit signalLabelDayShow(day);
}

void CalendarWidget::jumpToddate(int year, int month, int day)
{
    setYear(year);
    setMonth(month);
    setDay(day);

    initDate();
    initData();
}

void CalendarWidget::initWidget()
{
    this->setMinimumSize(200, 120);
    this->setObjectName("widgetDate");

    labelShowDay = new QLabel(this);
    labelShowDay->setObjectName("labelShowDay");
    labelShowDay->setFixedSize(100, 100);
    labelShowDay->setText(tr("%1").arg(m_nDay));
    labelShowDay->setAlignment(Qt::AlignCenter);
    connect(this, &CalendarWidget::signalLabelDayShow,
            &CalendarWidget::slotSetLabelDayShow);

    verLayoutDate = new QVBoxLayout(this);
    verLayoutDate->setContentsMargins(0, 0, 0, 0);

    verLayoutDate->addWidget(labelShowDay);

    widgetTitle = new QWidget(this);
    widgetTitle->setObjectName("widgetTitle");
    widgetTitle->setMinimumHeight(20);

    // 标题上添加label来显示日期
    labelTitle = new QLabel(widgetTitle);
    labelTitle->setObjectName("labelTitle");
    labelTitle->setText(tr("2017年 04 月"));
    labelTitle->setWordWrap(true);
    labelTitle->setAlignment(Qt::AlignCenter);
    labelTitle->setStyleSheet("color: #646464; font-weight: bold;");


    widgetJump = new QWidget(this);
    widgetJumpLayout = new QGridLayout(widgetJump);
    widgetJump->setObjectName("widgetJump");

    btnPrevMonth = new QPushButton(widgetJump);
    btnPrevMonth->setText(tr("上月"));
    btnPrevMonth->setMinimumSize(20, 20);
    widgetJumpLayout->addWidget(btnPrevMonth, 0, 0);

    btnNextMonth = new QPushButton(widgetJump);
    btnNextMonth->setText(tr("下月"));
    btnNextMonth->setMinimumSize(20, 20);
    widgetJumpLayout->addWidget(btnNextMonth, 1, 0);

    connect(btnPrevMonth, SIGNAL(clicked(bool)),
            this, SLOT(sltShowPreMonth()));
    connect(btnNextMonth, SIGNAL(clicked(bool)),
            this, SLOT(sltShowNextMonth()));

    // 周几组件
    widgetWeek = new QWidget(this);
    widgetWeek->setObjectName("widgetWeek");

    gridLayoutWeek = new QGridLayout(widgetWeek);
    gridLayoutWeek->setContentsMargins(0, 9, 0, 0);

    horLayoutWeek = new QHBoxLayout();

    for (int i = 0; i < Week; i++) {
        labelWeek[i] = new QLabel(widgetWeek);
        labelWeek[i]->setText(s_strWeek[i]);
        labelWeek[i]->setObjectName("labelWeek");
        labelWeek[i]->setFixedSize(30, 20);
        labelWeek[i]->setAlignment(Qt::AlignRight);
        if ( (0 == (i % 7)) || (6 == (i % 7))) {
            labelWeek[i]->setProperty("weekend", true);
        }
        gridLayoutWeek->addWidget(labelWeek[i], i, 0);
    }

    widgetBody = new QWidget(this);

    gridLayoutBody = new QGridLayout(widgetBody);
    gridLayoutBody->setHorizontalSpacing(0);
    gridLayoutBody->setVerticalSpacing(0);
    gridLayoutBody->setContentsMargins(2, 2, 2, 2);

    // 先在labelDay[]设置好日期，等待构造
    for (int i = 0; i < 42; i++) {
        labelDay[i] = new DayLabel(widgetBody);
        labelDay[i]->setObjectName("labelDay");
        labelDay[i]->setAlignment(Qt::AlignCenter);
        labelDay[i]->setStyleSheet("#");
        labelDay[i]->setText(QString::number(i));
        if ( (0 == (i % 7)) || (6 == (i % 7))) {
            labelDay[i]->setProperty("weekend", true);
        }
        gridLayoutBody->addWidget(labelDay[i], i % 7, i / 7);
    }
    labelDay[10]->setSelected(true);
    horLayoutWeek->addWidget(widgetBody);
    horLayoutWeek->addWidget(widgetWeek);

    verLayoutDate->addWidget(widgetTitle);
    verLayoutDate->addLayout(horLayoutWeek);
    verLayoutDate->addWidget(widgetJump);
    this->setLayout(verLayoutDate);

}

// the DayLabelSHow
void CalendarWidget::slotSetLabelDayShow(int day)
{
    labelShowDay->setText(tr("%1").arg(day));
    qDebug() << day;
    fixedCurrDay = day;
}

void CalendarWidget::initDate()
{
    int nWeek = Date::getFirstDayOfWeek(m_nYear, m_nMonth);
    int nMonthDays = Date::getMonthDays(m_nYear, m_nMonth);
    int nPreMonDays = Date::getMonthDays(1==m_nMonth ? m_nYear - 1 : m_nYear, 1 == m_nMonth ? 12 : m_nMonth - 1);

    //显示日期
    labelTitle->setText((tr("%1 年 %2 月")
                         .arg(m_nYear, 2, 10, QChar('0'))
                         .arg(m_nMonth, 2, 10, QChar('0'))));
    if (0 == nWeek) {
        for (int i = 0; i < 7; i++) {
            labelDay[i]->showDay((nPreMonDays - 7 + i + 1));
            labelDay[i]->setColor(PREV_MONTH_DAY);
        }
        for (int i = 0; i < (42-nMonthDays -7); i++) {
            labelDay[nMonthDays + 7 + i]->showDay((i+1));
            labelDay[nMonthDays + 7 + i]->setColor(NEXT_MONTH_DAY);
        }
    }
    else {
        for (int i = 0; i < nWeek; i++) {
            labelDay[i]->showDay((nPreMonDays - nWeek + i + 1));
            labelDay[i]->setColor(PREV_MONTH_DAY);
        }

        for (int i = (nWeek + nMonthDays); i < 42; i++) {
            labelDay[i]->showDay((i-(nWeek + nMonthDays) + 1));
            labelDay[i]->setColor(NEXT_MONTH_DAY);
        }
    }

    if (FIRST) {
        initData();
        FIRST = 0;
    }

    int index = 0;
    int nProperty = 1;
    for (int i = nWeek; i < (nMonthDays + nWeek); i++) {
        index = 0 == nWeek ? (i+7) : i;
        labelDay[index]->showDay(i - nWeek + 1);
        nProperty = ((0 == (i % 7)) || (6 == (i % 7))) ? WEEKEND_DAY : CURR_MONTH_DAY;
        labelDay[index]->setColor(nProperty);
    }

    labelDay[m_nDay + nWeek - 1]->setColor(CURRENT_DAY);
    // emit signalDayChanged();
}

void CalendarWidget::initData()
{

    int nWeek = Date::getFirstDayOfWeek(m_nYear, m_nMonth);
    int nMonthDays = Date::getMonthDays(m_nYear, m_nMonth);

    QString strMemo;
    int index = 0;
    for (int i = nWeek; i < (nMonthDays + nWeek); i++) {
        index = 0 == nWeek ? (i+7) : i;
        strMemo = DataBase::queryNote(
                    QString("%1-%2-%3")
                    .arg(m_nYear, 4, 10, QChar('0'))
                    .arg(m_nMonth, 2, 10, QChar('0'))
                    .arg(i-nWeek + 1, 2, 10, QChar('0')));
        labelDay[index]->setMemo(strMemo);
        labelDay[index]->currDate = QString("%1-%2-%3").arg(m_nYear, 4, 10, QChar('0'))
                                                        .arg(m_nMonth, 2, 10, QChar('0'))
                                                        .arg(i-nWeek+1, 2, 10, QChar('0'));
    }
}

void CalendarWidget::sltDayClicked(int type, int day)
{
    if (PREV_MONTH_DAY == type) {
        sltShowPreMonth();
    }
    else if ( NEXT_MONTH_DAY==type) {
        sltShowNextMonth();
    }
    else if ((CURR_MONTH_DAY == type)
            || (WEEKEND_DAY == type)
            || (CURRENT_DAY == type)) {
        emit signalDayClicked(day);
    }
}

void CalendarWidget::sltShowPreMonth()
{
    m_nMonth--;
    if (m_nMonth < 1) {
        m_nMonth = 12;
        m_nYear--;
    }
    initDate();
    initData();
}

void CalendarWidget::sltShowNextMonth()
{
    m_nMonth += 1;
    if (m_nMonth > 12) {
        m_nMonth = 1;
        m_nYear += 1;
    }
    initDate();
    initData();
}

void CalendarWidget::changeTitle(QString &str)
{
    labelTitle->setText(str);
    labelTitle->setStyleSheet("color: #646464; font-weight: bold;");
}

void CalendarWidget::setLabelIcon(int nday)
{
    qDebug() << nday;
    int nWeek = Date::getFirstDayOfWeek(m_nYear, m_nMonth);
    labelDay[nday + nWeek - 1]->labelIcon->setVisible(true);
    update();
    repaint();
}

void CalendarWidget::slotRefreshMemo(QString &memo, int nday)
{
    int nWeek = Date::getFirstDayOfWeek(m_nYear, m_nMonth);
    labelDay[nday + nWeek - 1]->setMemo(memo);
    labelDay[nday + nWeek - 1]->labelIcon->setVisible(!memo.isEmpty());
}

void CalendarWidget::slotSetRedBox()
{
    int nWeek = Date::getFirstDayOfWeek(m_nYear, m_nMonth);
    labelDay[fixedCurrDay + nWeek - 1]->setSelected(true);
    repaint();
}

//void CalendarWidget::mouseMoveEvent(QMouseEvent *event)
//{
//    //在这里，重绘红框框，因为实在CalendarWidget里设置m_day来保存已点击的日期项
//    qDebug() << "CalendarWidget::mouseMoveEvent, ";
//    qDebug() << event->x();
//}

void CalendarWidget::enterEvent(QEvent *event)
{
//    qDebug("CalenderWidget::enterEvent, ");
//    int nWeek = Date::getFirstDayOfWeek(m_nYear, m_nMonth);
//    labelDay[m_nDay + nWeek - 1]->setSelected(true);
}

void CalendarWidget::leaveEvent(QEvent *event)
{
//    qDebug("CalenderWidget::leaveEvent, ");
//    int nWeek = Date::getFirstDayOfWeek(m_nYear, m_nMonth);
//    labelDay[m_nDay + nWeek - 1]->setSelected(true);
}

//DayLabel
#include <QMouseEvent>
#include <QEvent>
#include <QPixmap>

DayLabel::DayLabel(QWidget *parent)
    :QLabel(parent)
{
    bSelect = false;
    m_nDay = 0;
    m_strListMemo.clear();
    currDate = "";

    labelIcon = new QLabel(this);
    this->setFixedSize(28, 30);
    labelIcon->setFixedSize(QSize(12, 12));
    labelIcon->setPixmap(QPixmap(":images/eventindicator.png"));
    labelIcon->move(0, 0);
    labelIcon->setVisible(false);
}

//被选中的
bool DayLabel::getBSelect() const
{
    return bSelect;
}

//正在被选
void DayLabel::setSelected(bool value)
{
    this->bSelect = value;
    this->setStyleSheet(QString("background: #ffffff; border: 2px solid red; border-radius: 1px"));
}

//日期颜色
void DayLabel::setColor(const int &type)
{
    this->setProperty("type", type);
    this->setSelected(CURRENT_DAY == type);

    if ((PREV_MONTH_DAY == type) || (NEXT_MONTH_DAY == type)) {
        this->setStyleSheet("background: #ffffff; border-top: 1px solid #c3c3c3; border-left: 1px solid #c3c3c3; color: #999999; font-weight:bold;");
        labelIcon->setVisible(false);
    }
    else if (CURR_MONTH_DAY == type) {
        this->setStyleSheet("background: #646464; border-top: 1px solid #c3c3c3; border-left: 1px solid #c3c3c3; color: #999999; font-weight:bold;");
    }
    else if (WEEKEND_DAY == type) {
        this->setStyleSheet("background: #00f0f0; border-top: 1px solid #c3c3c3; border-left: 1px solid #c3c3c3; color: #999999; font-weight:bold;");
    }
}

void DayLabel::showDay(int nDay)
{
    QString strText = QString::number(nDay);
    this->m_nDay = nDay;

    this->setText(strText);
}

//设置日记
void DayLabel::setMemo(QString memo)
{
    if (!memo.isEmpty()) {
        m_strListMemo = memo;
    }
    else {
        m_strListMemo = "";
    }

    labelIcon->setVisible(!memo.isEmpty());
}

//进入的效果
void DayLabel::enterEvent(QEvent *event)
{
    int nProperty = this->property("type").toInt();
    orig = this->styleSheet();
    if (PREV_MONTH_DAY == nProperty || NEXT_MONTH_DAY == nProperty)
        return ;
    this->setStyleSheet("color: #ffff00; background: #c8b9a6; border-top: 1px solid #c3c3c3; border-left: 2px solid #c3c3c3; font:24px;");
    QLabel::enterEvent(event);
    emit signalSetRedBox();
    qDebug("DayLabel::enterEvent, ");
}

//离开的效果
void DayLabel::leaveEvent(QEvent *event)
{
    int nProperty = this->property("type").toInt();
    if (PREV_MONTH_DAY == nProperty || NEXT_MONTH_DAY == nProperty)
        return ;
    this->setStyleSheet(orig);
    //if (this->bSelect) {
    //    this->setStyleSheet("border: 2px solid red");
    //}
    // 转发
    QLabel::leaveEvent(event);
    emit signalSetRedBox();
    qDebug("DayLabel::leaveEvent, ");
}

//单击事件
void DayLabel::mousePressEvent(QMouseEvent *event)
{
    // 点击有几个事件：
    // 1. 设置Editor的内容
    // 2. 设置大的那个日期label
    // 3. 设置颜色变化，红框框
    // 框框堆叠主要原因：在dayLabel上进行的操作会只在那个label更新
    // 要整个更新就要用，initDate()将界面重绘，但是重绘整个日历界面
    // 很消耗资源的, 权当是一个小小的bug吧，不修改了
    //
    QString str = m_strListMemo;
    if (event->button() != Qt::LeftButton)
        return ;

    emit signalCurrDay(m_nDay);
    // emit signalSetRedBox();

    // emit signalMemo(str);
    // emit signalCurrDate(currDate);

    setSelected(true);
    // QLabel::mousePressEvent(event);
}

void DayLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    QString str = m_strListMemo;
    if (event->button() != Qt::LeftButton)
        return ;
    // signal to RightDownWidget
    emit signalMemo(str);
    emit signalCurrDate(currDate);
    // the label
    setSelected(true);
}

//DateWidget
#include <QMessageBox>
#include "database/database.h"

//构造函数
DateWidget::DateWidget(QWidget *parent)
    :QWidget(parent)
{
    initWidget();
    initStyle();
}

//
DateWidget::~DateWidget()
{

}

//初始化窗口界面
void DateWidget::initWidget()
{
    this->setFixedSize(210, 630);
    //日记组件
    calendar = new CalendarWidget(this);

    horLatoutWidget = new QHBoxLayout();

    groupBoxBottom = new QGroupBox();
    groupBoxBottom->setContentsMargins(0, 0, 0, 0);

     //日期跳转
    labelYearJump = new QLabel(groupBoxBottom);
    labelMonthJump = new QLabel(groupBoxBottom);
    labelDayJump = new QLabel(groupBoxBottom);

    labelYearJump->setMaximumHeight(30);
    labelMonthJump->setMaximumHeight(30);
    labelDayJump->setMaximumHeight(30);

    labelYearJump->setObjectName("labelCommon");
    labelMonthJump->setObjectName("labelCommon");
    labelDayJump->setObjectName("labelCommon");

    labelYearJump->setText(tr("年: "));
    labelMonthJump->setText(tr("月: "));
    labelDayJump->setText(tr("日: "));

    //选择日期
    editYearJump = new QLineEdit(groupBoxBottom);
    editMonthJump = new QLineEdit(groupBoxBottom);
    editDayJump = new QLineEdit(groupBoxBottom);

    editYearJump->setMaximumWidth(40);
    editYearJump->setMinimumWidth(30);
    editMonthJump->setMaximumWidth(40);
    editMonthJump->setMinimumWidth(30);
    editDayJump->setMaximumWidth(40);
    editDayJump->setMinimumWidth(30);

    editYearJump->setText(QDateTime::currentDateTime().toString("yyyy"));
    editMonthJump->setText(QDateTime::currentDateTime().toString("MM"));
    editDayJump->setText(QDateTime::currentDateTime().toString("dd"));

    // 消息条
    labelMsg = new QLabel(this);

    //跳转按钮
    btnDateJump = new QPushButton(this);
    btnToday = new QPushButton(this);

    btnDateJump->setText(tr("跳转"));
    btnToday->setText(tr("回到今天"));

    //组件添加
    horLayoutJump = new QHBoxLayout(groupBoxBottom);
    horLayoutJump->setSpacing(0);

    horLayoutJump->addWidget(btnDateJump);
    horLayoutJump->addWidget(btnToday);

    layoutJump = new QFormLayout();

    layoutJump->addRow(labelYearJump, editYearJump);
    layoutJump->addRow(labelMonthJump, editMonthJump);
    layoutJump->addRow(labelDayJump, editDayJump);

    verLayoutAll = new QVBoxLayout(this);
    verLayoutAll->setContentsMargins(0, 0, 0, 0);
    //verLayoutAll->addSpacing(20);
    verLayoutAll->addWidget(calendar);
    verLayoutAll->addWidget(labelMsg);
    verLayoutAll->addLayout(layoutJump);
    verLayoutAll->addWidget(groupBoxBottom);
    verLayoutAll->addSpacing(40);

    //信号槽连接
    connect(btnDateJump, SIGNAL(clicked(bool)),
            this, SLOT(sltDateJump()));
    connect(btnToday, SIGNAL(clicked(bool)),
            this, SLOT(sltBack2todday()));

}

//美化
void DateWidget::initStyle()
{
    QString strStyle = "";
    strStyle += QString("QWidget#widgetWeek {border-top: 1px solid #eeee00; border-bottom: 1px solid #ffff00;background-color: #e0eee0; border-radius: 3px; text-align: center}");
    strStyle += QString("QLabel#labelShowDay {background-color: #646464; border-radius: 5px; border-left: 2px solid #ffff00; font-weight: bold; font:50px} ");
    strStyle += QString("QLineEdit {border: 1px solid #ffffff; border-radius: 2px;}");

    this->setStyleSheet(strStyle);
}

//跳转槽函数
void DateWidget::sltDateJump()
{
    int year = editYearJump->text().toInt();
    int month = editMonthJump->text().toInt();
    int day = editDayJump->text().toInt();
    int nMaxSet = 0;

    if ((year > 2040) || (year < 1970)) {
        QMessageBox::information(this, tr("提示"),
                                 tr("输入错误"));
        editYearJump->setFocus();
        return ;
    }

    if ((month > 12) || (month < 1)) {
        editMonthJump->setFocus();
        QMessageBox::information(this, tr("提示"),
                                 tr("输入错误"));
        return ;
    }

    nMaxSet = Date::getMonthDays(year, month);

    if ((day > nMaxSet) || day < 1) {
        editDayJump->setFocus();
        QMessageBox::information(this, tr("提示"), tr("输入错误"));
        return ;
    }
    calendar->jumpToddate(year, month, day);
}

//回到今天槽函数
void DateWidget::sltBack2todday()
{
    int year = QDate::currentDate().year();
    int month = QDate::currentDate().month();
    int day = QDate::currentDate().day();

    calendar->jumpToddate(year, month, day);
}

void DateWidget::sltDisplaymsg(const QString &msg)
{
    labelMsg->setText(msg);
    m_timer = new QTimer(this);
    m_timer->start(2000);
    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this, &DateWidget::sltStopTimer);
}

void DateWidget::sltStopTimer()
{
    labelMsg->setText(" ");
    m_timer->stop();
}

void DateWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
