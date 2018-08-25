#ifndef RIGHTUPWIDGET_H
#define RIGHTUPWIDGET_H

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
    QString picPath;
    void showSuitableSize(QPainter *);

signals:
    void signalSetPicPath();

public Q_SLOTS:
    void slotLoadImage();
    void slotLoadImage(QString file_path);
    void slotSetPicPath();

private:
    QPixmap m_currPixmap;

    QPushButton *labelprev;
    QPushButton *labelnext;
protected:
    void paintEvent(QPaintEvent *);
    void drawBackground(QPainter *);
    void drawImage(QPainter *);
};

#endif // RIGHTUPWIDGET_H
