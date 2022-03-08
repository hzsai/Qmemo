#ifndef QREPLYTIMEOUT_H
#define QREPLYTIMEOUT_H

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

/*
    来 自https://blog.csdn.net/liang19890820/article/details/53204396
    作者：一去二、三
*/

#include <QObject>
#include <QTimer>
#include <QNetworkReply>

class QReplyTimeout : public QObject
{
    Q_OBJECT
public:
    QReplyTimeout(QNetworkReply* reply, const int timeout);

signals:
    void timeout(QString msg); // 超时信号,重写了

private slots:
    void onTimeout();
};

#endif // QREPLYTIMEOUT_H
