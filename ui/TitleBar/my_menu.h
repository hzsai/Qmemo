#ifndef MY_MENU_H
#define MY_MENU_H
#include <QAction>
#include "ui/Common/menu.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif


class MyMenu : public Menu
{
public:
    explicit MyMenu(QWidget *parent = 0);
    ~MyMenu();

    void translator();
    void choiceFont();

signals:
    void signalSetPicPath();

public slots:
    void slotQuit();
    void slotShowAboutAuthur();
    void slotOpenPic();
    void slotAboutAuthur();
    void slotAboutQt();
    void slotSetPicPath();
    void slotShowTodayWeather();
    void slotHelp();

public:
    void initForm();
    void initWidget();
    void initConnect();

public:

    //about
    Menu *m_menuAbout{nullptr};
    QAction *aboutAuthur{nullptr};
    QAction *aboutQt{nullptr};
    //setting
    QAction *m_actionOpenPic{nullptr};

    QAction *m_help{nullptr};

    //setting action
    Menu *m_menuSetting{nullptr};
    QAction *m_actionSettingFont{nullptr};
    //quit action
    QAction *m_actionQuit{nullptr};
    QString picPath;

    // weather
    QAction *aboutTodayWeather{nullptr};

    // 每日一图
    QAction *getEveryDayPicture{nullptr};
};

#endif // MY_MENU_H
