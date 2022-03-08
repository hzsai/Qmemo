#include "menu.h"
#include <QCursor>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

Menu::Menu(QWidget *parent)
    :QMenu(parent)
{
    this->setStyleSheet("background-color:rgba(255, 255, 255, 100%);"
                        "selection-background-color:rgba(0, 128, 180, 80%)");

}
Menu::~Menu()
{

}

bool Menu::event(QEvent *event)
{
    return QWidget::event(event);
}

void Menu::slotMenuVisiable()
{
    QPoint pos = QCursor::pos();
    int width = 0;
    int height = 0;
    int maxWidth = QApplication::desktop()->width();
    int maxHeight = QApplication::desktop()->height();
    this->setVisible(true);

    //设置菜单的合理 显：
    if (pos.x() + this->width() > maxWidth) {
        width = -(this->width());
    }
    if (pos.y() + this->height() > maxHeight) {
        height = -(this->height());
    }

    this->move(pos.x()+width, pos.y() + height);
}
