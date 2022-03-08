#include "qreplytimeout.h"

QReplyTimeout::QReplyTimeout(QNetworkReply* reply, const int timeout)
    :QObject(reply)
{
    Q_ASSERT(reply);
    if (reply && reply->isRunning()) {
        QTimer::singleShot(timeout, this, SLOT(onTimeout()));
    }
}

void QReplyTimeout::onTimeout()
{
    QNetworkReply* reply = static_cast<QNetworkReply*>(parent());
    if (reply->isRunning()) {
        reply->abort();
        reply->deleteLater();
        emit timeout(tr("请求超 时！！！"));
    }
}
