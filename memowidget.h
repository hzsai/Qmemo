#ifndef MEMOWIDGET_H
#define MEMOWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QNetworkAccessManager>

#include "plugins/iciba_word.h"

class TitleWidget;
class RightUpWidget;
class RightDownWidget;
class DateWidget;
class DataBase;

class QVBoxLayout;
class QHBoxLayout;
class QNetworkReply;
class QNetworkAccessManager;

// 请求类型
typedef enum {
    REQ_JSON,
    REQ_PIC,
    REQ_TEXT,
    REQ_FILE
} REQ_TYPE;

class MemoWidget : public QFrame
{
    Q_OBJECT
public:
    MemoWidget(QWidget *parent = 0);
    ~MemoWidget();
private:
    void initForm();
    void initWidget();
    void initConnect();
    void initBaseInfo();

    // 创建请求
    void createRequest();
    void createRequest(QString url);
    void createRequest(QString url, int type);

signals:
    void signalMax(int);
    void signalDealWithJsonTypeFinished(QString url, int type);
    void signalDealWithFileTypeFinished(QString file_path);
    void signalParseJsonError(QString msg);
    void signalCommonMsg(QString msg);

public slots:
    void slotClose();

    void slotShowMax();
    void slotShowMin();

    // 处理来自QNetAccessManager的返回数据
    void slotDealWithRequest();
    void slotDealWithFileType();
    void slotDealWithJsonType();
    void slotDealWithPicType();
    void slotDealWithTextType();


protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);

private:

    bool m_isMax;
    bool m_isPressed;
    QPoint m_point;
    QRect m_location;

    TitleWidget *m_title;
    RightDownWidget *m_rightDownWidget;
    RightUpWidget *m_rightUpWidget;
    DateWidget *m_leftDateWidget;

    QHBoxLayout *m_midLayout;

    QVBoxLayout *m_maintLayout;
    QVBoxLayout *m_rightLayout;

    DataBase *database;

    IcibaWord *icibaword;

    QNetworkAccessManager *qnam;
    QNetworkReply *reply;

};

#endif // MEMOWIDGET_H
