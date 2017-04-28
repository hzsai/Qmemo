#ifndef RIGHTUPWIDGET_H
#define RIGHTUPWIDGET_H

#include <QWidget>
#include <QObject>


class RightUpWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RightUpWidget(QWidget *parent = 0);
    ~RightUpWidget();

    void initForm();
    void initConnect();
    void loadImage(QString path);
    QString picPath;
    void showSuitableSize(QPainter *);

signals:
    void signalSetPicPath();

public Q_SLOTS:
    void slotLoadImage();
    void slotSetPicPath();

private:
    QPixmap m_currPixmap;
protected:
    void paintEvent(QPaintEvent *);
    void drawBackground(QPainter *);
    void drawImage(QPainter *);
};

#endif // RIGHTUPWIDGET_H
