#ifndef DATEWIDGET_H
#define DATEWIDGET_H

#include <QWidget>
#include <QLabel>

QT_BEGIN_NAMESPACE
class QPushButton;
class QComboBox;
class QGroupBox;
class QLineEdit;
class QDate;
class QTimer;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class QFormLayout;

class Event;
class DataBase;
QT_END_NAMESPACE

// 周日~周六
typedef enum {
    Sunday,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Fridday,
    Saturday,
    Week
} E_WEEK;

// 日历界面点击
typedef enum {
    PREV_MONTH_DAY,
    NEXT_MONTH_DAY,
    CURR_MONTH_DAY,
    WEEKEND_DAY,
    CURRENT_DAY,
} E_DAY;




// 每一个显示日期的格子组件
class DayLabel : public QLabel {
    Q_OBJECT
public:
    explicit DayLabel(QWidget *parent = 0);

    bool bSelect;
    QString orig;
    QString currDate;
    bool getBSelect() const;
    void setSelected(bool value);
    void setColor(const int &type);
    void showDay(int nDay);

    int m_nDay;
    QLabel *labelIcon;
    void setMemo(QString memo);
signals:
    void signalMemo(QString &);
    void signalCurrDate(QString &);
    void signalCurrDay(int day);
    void signalSetRedBox();
private:
    QString m_strListMemo;
protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

};


// 整体的日历组件，调用后是一整块的
class CalendarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CalendarWidget(QWidget *parent = 0);
    ~CalendarWidget();

    int year() const;
    void setYear(int year);

    int month() const;
    void setMonth(int month);

    int day() const;
    void setDay(int day);
    int selected;

    void jumpToddate(int yeay, int month, int day);

public:
    QWidget *widgetBody;
    DayLabel *labelDay[42];
    void initDate();
    void initData();

signals:

    void signalDayClicked(int);
    void signalDayChanged();
    void signalDayDbClicked();

    void signalLabelDayShow(int);

public slots:
    void changeTitle(QString &str);
    void slotSetLabelDayShow(int day);
    void setLabelIcon(int );
    void slotRefreshMemo(QString &, int);
    void slotSetRedBox();

private:
    QVBoxLayout *verLayoutCalender;
    QHBoxLayout *horLayoutJump;
    QVBoxLayout *verLayoutJump;
    QVBoxLayout *verLayoutDate;
    QVBoxLayout *verLayoutWeek;
    QWidget *widgetJump;

    QVBoxLayout *verlayout1Widget;
    QVBoxLayout *verlayout2Widget;
    QVBoxLayout *verlayout3Widget;

    QGridLayout *widgetJumpLayout;

    QWidget *widgetTitle;
    QLabel *labelTitle;
    QPushButton *btnPrevMonth;
    QPushButton *btnNextMonth;
    QHBoxLayout *horLayoutTitle;

    QWidget *widgetWeek;
    QLabel *labelWeek[Week];
    QHBoxLayout *horLayoutWeek;

    QGridLayout *gridLayoutBody;
    QGridLayout *gridLayoutWeek;

    QWidget *widgetRight;

    QLabel *labelShowToday;
    QLabel *labelShowWeek;
    QLabel *labelShowDay;
    QLabel *labelShowNYear;
    QLabel *labelShowLunarDate;
    QLabel *labelSpacer;
    QLabel *labelScheduleTitle;
    QLabel *labelSchedule;

    QVBoxLayout *verlayoutWidgetRight;
    QHBoxLayout *horLayoutGlobal;



private:
    int m_nYear;
    int m_nMonth;
    int m_nDay;

    int fixedCurrDay;
private:
    void initWidget();

private slots:
    void sltDayClicked(int type, int day);
    void sltShowPreMonth();
    void sltShowNextMonth();

protected:
    // void mouseMoveEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
};

// 明显，在一开始，我没有理解这是什么，必须重构了
class DateWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DateWidget(QWidget *parent = 0);
    ~DateWidget();

public:
    // 日历组件
    CalendarWidget *calendar;

    QWidget *widgetRight;

    QVBoxLayout *verlayoutWidgetRight;
    QHBoxLayout *horLatoutWidget;

    QFormLayout *layoutJump;

    // 年月显示
    QGroupBox *groupBoxBottom;
    QLabel *labelYearJump;
    QLabel *labelMonthJump;
    QLabel *labelDayJump;

    // 跳转输入
    QLineEdit *editYearJump;
    QLineEdit *editMonthJump;
    QLineEdit *editDayJump;

    // 再更新：这里最好用消息队列来处理显示信息哦，暂时留坑
    // 消息显示条
    QLabel *labelMsg;

    // 简单及时
    QTimer *m_timer;

    // 两个跳转按钮
    QPushButton *btnDateJump;
    QPushButton *btnToday;

    // 布局
    QHBoxLayout *horLayoutJump;
    QVBoxLayout *verLayoutAll;

signals:

private:
    void initWidget();
    void initStyle();

private slots:
    void sltDateJump();
    void sltBack2todday();

public slots:
    // 用于显示提示信息消息槽
    void sltDisplaymsg(const QString &msg);
    void sltStopTimer();

protected:
    void paintEvent(QPaintEvent *);
};

#endif
