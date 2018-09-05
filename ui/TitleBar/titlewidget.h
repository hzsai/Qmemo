#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QObject>
#include <QWidget>

class QPushButton;
class QLabel;
class QHBoxLayout;
class MyMenu;
class QMediaPlayer;
class QMediaPlaylist;

typedef enum TbnStatus {
    BtnMax = 0,
    BtnMin
}TBNSTATUS;

class TitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TitleWidget(QWidget *parent = 0);
    ~TitleWidget();

    void Translator();

signals:
    void signalClose();
    void signalMax();
    void signalMin();
    void signalClickOnChoice();
    void signalPlay(int);
    //void signalQuit();

    //void signalShowOrHideWidget(int);

public slots:
    void slotChangeBtnStatus(int status);
    void slotUpdateTime();
    void slotPrePlay();
    void slotPlay(int );
    void slotSetEveryDayEnglish(const QString &parse);
private:
    void initForm();
    void initWidget();
    void initConnect();
    // static void initTimeLabel(QWidget *parent = 0);

public:
    QLabel *m_TbnTime;
    QPushButton *m_TbnMenu;
    QPushButton *m_TbnMenu_max;
    QPushButton *m_TbnMenu_min;
    QPushButton *m_TbnMenu_close;

    QHBoxLayout *m_mainLayout;
    MyMenu *m_menu;
    QTimer *m_Timer;
    QTimer *m_TimerPlay;
    QMediaPlayer *player;
    QMediaPlaylist *playList;

    // 天气
    QHBoxLayout *m_horShowWeather;
    QLabel *WeatherData;

    // 每日一句英语
    QLabel *everydayEnglish;

};

#endif // TITLEWIDGET_H
