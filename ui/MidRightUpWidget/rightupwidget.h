#ifndef RIGHTUPWIDGET_H
#define RIGHTUPWIDGET_H

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QWidget>
#include <QObject>

class QPushButton;

class RightUpWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RightUpWidget(QWidget *parent = 0);
    ~RightUpWidget();

    void initForm();
    void initConnect();
    void loadImage(QString path);
    void showSuitableSize(QPainter *);

    QString picPath;
    QPushButton *labelprev;
    QPushButton *labelnext;

signals:
    void signalSetPicPath();
    void signalPrevPic();
    void signalNextPic();

public Q_SLOTS:
    void slotLoadImage();
    void slotLoadImage(QString file_path);
    void slotSetPicPath();

private:
    QPixmap m_currPixmap;

protected:
    void paintEvent(QPaintEvent *);
    void drawBackground(QPainter *);
    void drawImage(QPainter *);
};

#endif // RIGHTUPWIDGET_H
