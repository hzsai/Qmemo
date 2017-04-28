#ifndef MY_MENU_H
#define MY_MENU_H
#include <QAction>
#include "ui/Common/menu.h"


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
    void slotHelp();

public:
    void initForm();
    void initWidget();
    void initConnect();

public:

    //about
    Menu *m_menuAbout;
    QAction *aboutAuthur;
    QAction *aboutQt;
    //setting
    QAction *m_actionOpenPic;

    QAction *m_help;

    //setting action
    Menu *m_menuSetting;
    QAction *m_actionSettingFont;
    //quit action
    QAction *m_actionQuit;
    QString picPath;
};

#endif // MY_MENU_H
