#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QObject>
#include <QWidget>

class IcibaWord;
class QPushButton;
class QLabel;
class QHBoxLayout;
class MyMenu;
class QMediaPlayer;
class QMediaPlaylist;
class QSound;

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
    void sig_play();

    //void signalShowOrHideWidget(int);

public slots:
    void slotChangeBtnStatus(int status);
    void slotUpdateTime();
    void slotPrePlay();
    void slotPlay(int );
    void slotSetEveryDayEnglish(const IcibaWord &parse);
    void slt_playerPlay();
private:
    void initForm();
    void initWidget();
    void initConnect();
    // static void initTimeLabel(QWidget *parent = 0);

public:
    QLabel* m_TbnTime{nullptr};
    QPushButton *m_TbnMenu{ nullptr };
    QPushButton *m_TbnMenu_max{ nullptr };
    QPushButton *m_TbnMenu_min{ nullptr };
    QPushButton *m_TbnMenu_close{ nullptr };

    QHBoxLayout *m_mainLayout{ nullptr };
    MyMenu *m_menu{ nullptr };
    QTimer *m_Timer{ nullptr };
    QTimer *m_TimerPlay{ nullptr };
    QMediaPlayer *player{ nullptr };
    QMediaPlaylist *playList{ nullptr };
    int playTimes_{ 0 };

    // 天气
    QHBoxLayout *m_horShowWeather{ nullptr };
    QLabel *WeatherData{ nullptr };

    // 每日一句英语
    QLabel *everydayEnglish{ nullptr };

};

#endif // TITLEWIDGET_H
