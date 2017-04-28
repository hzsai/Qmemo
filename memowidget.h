#ifndef MEMOWIDGET_H
#define MEMOWIDGET_H

#include <QWidget>
#include <QFrame>

class TitleWidget;
class RightUpWidget;
class RightDownWidget;
class DateWidget;
class DataBase;

class QVBoxLayout;
class QHBoxLayout;


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

Q_SIGNALS:
    void signalMax(int);
public Q_SLOTS:
    void slotClose();

    void slotShowMax();
    void slotShowMin();


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

};

#endif // MEMOWIDGET_H
