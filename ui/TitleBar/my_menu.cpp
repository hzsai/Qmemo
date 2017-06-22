#include "my_menu.h"
#include "memowidget.h"
#include "ui/Common/menu.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QFontDialog>
#include "ui/MidRightDownWidget/rightdownwidget.h"
#include "ui/MidRightUpWidget/rightupwidget.h"


MyMenu::MyMenu(QWidget *parent)
    :Menu(parent)
{
    this->initWidget();
    this->initForm();
    this->initConnect();
}

MyMenu::~MyMenu()
{

}

void MyMenu::translator()
{
    m_actionOpenPic->setText(tr("更换图片"));
    m_actionQuit->setText(tr("退出"));
    m_actionSettingFont->setText(tr("设置字体"));

    aboutAuthur->setText(tr("关于Authur"));
    aboutQt->setText(tr("关于Qt"));
}

void MyMenu::initForm()
{

}

void MyMenu::initWidget()
{
    m_menuAbout = new Menu(this);
    m_menuAbout->setTitle("关于");
    m_menuAbout->setStyleSheet("color: #646464; font-weight: bold;");
    aboutAuthur = new QAction(this);
    aboutAuthur->setText(tr("关于hzsai"));
    aboutQt = new QAction(this);
    aboutQt->setText(tr("关于Qt"));
    //添加关于二级菜单
    m_menuAbout->addAction(aboutAuthur);
    m_menuAbout->addAction(aboutQt);

    //选择图片、设置字体、退出
    m_menuSetting = new Menu(this);
    m_menuSetting->setTitle(tr("设置"));
    m_menuSetting->setStyleSheet("color: #646464; font-weight: bold;");

    m_help = new QAction(this);
    m_help->setText(tr("帮助"));
    m_help->setToolTip(tr("简单帮助文档"));

    m_actionOpenPic = new QAction(this);
    m_actionOpenPic->setText(tr("设置图片"));
    m_actionQuit = new QAction(this);
    m_actionQuit->setText(tr("退出"));
    m_actionSettingFont = new QAction(this);
    m_actionSettingFont->setText(tr("设置字体"));

    //设置二级菜单
    m_menuSetting->addAction(m_actionSettingFont);

    //菜单添加图片实例
    //m_menuAbout->setIcon(QIcon("qrc:images/menuSetting.png"));
    //m_actionQuit->setIcon(QIcon("qrc:images/close.png"));

    this->addAction(m_help);
    this->addSeparator();
    this->addAction(m_actionOpenPic);
    this->addSeparator();
    this->addMenu(m_menuSetting);
    this->addSeparator();
    this->addMenu(m_menuAbout);
    this->addSeparator();

    this->addAction(m_actionQuit);

}

void MyMenu::initConnect()
{
    connect(aboutQt, &QAction::triggered,
            this, &QApplication::aboutQt);
    connect(aboutAuthur, &QAction::triggered,
            this, &MyMenu::slotAboutAuthur);
    connect(m_help, &QAction::triggered,
            this, &MyMenu::slotHelp);
}


void MyMenu::slotAboutQt()
{
    //Qt自身About组件
    QApplication::aboutQt();
}

void MyMenu::slotAboutAuthur()
{
    QMessageBox box;
    box.information(this, tr("关于"), tr("来自<b>hzsai</b><br>"), QMessageBox::Yes);
}

void MyMenu::slotHelp()
{
    QMessageBox box;
    box.information(this, tr("帮助"), tr("帮助<br>(1) 在相应日期下写日记并且保存就行啦，界面就显示了全部功能。<br>"
                                       "(2) 菜单设置图片和字体。半点和整点会报时(⊙o⊙)哦^_^<br>"
                                       "(3) 删除掉执行文件所在目录下的<strong>qmemo.ini</strong>文件可以再次看到那张帅帅的图片(⊙o⊙)哦<br>"
                                       "(4) 查看日记前先<strong>双击</strong>和<strong>单击</strong>某个日期哦<br>"
                                       "(5) 图标来自www.easyicon.net,非常感谢！！！"), QMessageBox::Yes);
}
