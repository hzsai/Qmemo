#ifndef DATEWIDGET_H
#define DATEWIDGET_H

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

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

typedef enum {
    PREV_DAY,
    NEXT_DAY
} PN_DAY;




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
    QLabel *labelIcon{nullptr};
    void setMemo(QString memo);

public:
    void pre_setRightDownWidget();
signals:
    void signalMemo(QString &);
    void signalCurrDate(QString &);
    void signalCurrDay(int day);
    void signalSetRedBox();
    void sig_setFixedCurrDay(QString &date);
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


    DayLabel* currDayLabel() const;
    void jumpToddate(int yeay, int month, int day);

public:
    QWidget *widgetBody{nullptr};
    DayLabel *labelDay[42]{nullptr};
    void initDate();
    void initData();

signals:

    void signalDayClicked(int);
    void signalDayChanged();
    void signalDayDbClicked();

    void signalLabelDayShow(int);
    void signalDayFinished(int);

public slots:
    void changeTitle(QString &str);
    void slotSetLabelDayShow(int day);
    void slt_setFixedCurrDay(QString &date);
    void setLabelIcon(int );
    void slotRefreshMemo(QString &, int);
    void slotSetRedBox();

private:
    QVBoxLayout *verLayoutCalender{nullptr};
    QHBoxLayout *horLayoutJump{nullptr};
    QVBoxLayout *verLayoutJump{nullptr};
    QVBoxLayout *verLayoutDate{nullptr};
    QVBoxLayout *verLayoutWeek{nullptr};
    QWidget *widgetJump{nullptr};

    QVBoxLayout *verlayout1Widget{nullptr};
    QVBoxLayout *verlayout2Widget{nullptr};
    QVBoxLayout *verlayout3Widget{nullptr};

    QGridLayout *widgetJumpLayout{nullptr};

    QWidget *widgetTitle{nullptr};
    QLabel *labelTitle{nullptr};
    QPushButton *btnPrevMonth{nullptr};
    QPushButton *btnNextMonth{nullptr};
    QHBoxLayout *horLayoutTitle{nullptr};

    QWidget *widgetWeek{nullptr};
    QLabel *labelWeek[Week]{nullptr};
    QHBoxLayout *horLayoutWeek{nullptr};

    QGridLayout *gridLayoutBody{nullptr};
    QGridLayout *gridLayoutWeek{nullptr};

    QWidget *widgetRight{nullptr};

    QLabel *labelShowToday{nullptr};
    QLabel *labelShowWeek{nullptr};
    QLabel *labelShowDay{nullptr};
    QLabel *labelShowNYear{nullptr};
    QLabel *labelShowLunarDate{nullptr};
    QLabel *labelSpacer{nullptr};
    QLabel *labelScheduleTitle{nullptr};
    QLabel *labelSchedule{nullptr};

    QVBoxLayout *verlayoutWidgetRight{nullptr};
    QHBoxLayout *horLayoutGlobal{nullptr};



private:
    int m_nYear;
    int m_nMonth;
    int m_nDay;

    int fixedCurrDay;
private:
    void initWidget();

public slots:
    void sltDayClicked(int type, int day);
    void sltShowPreMonth();
    void sltShowNextMonth();
    void sltShowPreDay();
    void sltShowNextDay();

protected:
    // void mouseMoveEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
};

class DateWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DateWidget(QWidget *parent = 0);
    ~DateWidget();

public:
    // 日历组件
    CalendarWidget *calendar{nullptr};

    QWidget *widgetRight{nullptr};

    QVBoxLayout *verlayoutWidgetRight{nullptr};
    QHBoxLayout *horLatoutWidget{nullptr};

    QFormLayout *layoutJump{nullptr};

    // 年月显示
    QGroupBox *groupBoxBottom{nullptr};
    QLabel *labelYearJump{nullptr};
    QLabel *labelMonthJump{nullptr};
    QLabel *labelDayJump{nullptr};

    // 跳转输入
    QLineEdit *editYearJump{nullptr};
    QLineEdit *editMonthJump{nullptr};
    QLineEdit *editDayJump{nullptr};

    // 再更新：这里最好用消息队列来处理显示信息哦，暂时留坑
    // 消息显示条
    QLabel *labelMsg{nullptr};

    // 简单及时
    QTimer *m_timer{nullptr};

    // 两个跳转按钮
    QPushButton *btnDateJump{nullptr};
    QPushButton *btnToday{nullptr};

    // 布局
    QHBoxLayout *horLayoutJump{nullptr};
    QVBoxLayout *verLayoutAll{nullptr};

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
