#include "memowidget.h"
#include "ui/TitleBar/titlewidget.h"
#include "ui/MidLeftWidget/datewidget.h"
#include "ui/MidRightDownWidget/rightdownwidget.h"
#include "ui/MidRightUpWidget/rightupwidget.h"
#include "ui/TitleBar/my_menu.h"
#include "database/database.h"


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

bool g_WidgetIsMin = false;

MemoWidget::MemoWidget(QWidget *parent)
    :QFrame(parent)
{
    database = new DataBase(this);
    this->initWidget();
    this->initForm();
    this->initConnect();
}

MemoWidget::~MemoWidget()
{

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
    //
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
    }
    // refresh LabelIcon
    connect(m_rightDownWidget, &RightDownWidget::signalSetIcon,
           m_leftDateWidget->calendar,&CalendarWidget::setLabelIcon);
    // refresh
    connect(m_rightDownWidget->m_save, &QPushButton::clicked,
            m_leftDateWidget->calendar, &CalendarWidget::initDate);
    // store back
    connect(m_rightDownWidget, &RightDownWidget::signalRightDownWidgetSave,
            m_leftDateWidget->calendar, &CalendarWidget::slotRefreshMemo);

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
