#include "memowidget.h"
#include <QApplication>
#include <QSettings>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //日历记事本实例
    MemoWidget memo;
    memo.setWindowIconText(QObject::tr("记事本"));
    memo.setWindowTitle(QObject::tr("记事本"));
    // set fontSize
    QFont font = app.font();
    font.setPointSize(9);
    app.setFont(font);

    // 无聊新构想：在日历界面三连可以消除，，，
    memo.show();

    return app.exec();
}


