#include "memowidget.h"
#include <QApplication>
#include <QSettings>
#include <QFont>
#include <QStyleFactory>

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

    memo.show();

    return app.exec();
}


