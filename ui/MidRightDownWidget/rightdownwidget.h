#ifndef RIGHTDOWNWIDGET_H
#define RIGHTDOWNWIDGET_H

#include <QWidget>

class QTextEdit;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;

class RightDownWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RightDownWidget(QWidget *parent = 0);
    ~RightDownWidget();
    QTextEdit *m_textEdit;
    bool maybeSave();
    QPushButton *m_save;
    QString currDate;			//用于外部接入数据。

signals:
    void signalSetIcon(int );
    void signalRightDownWidgetSave(QString &, int);

public slots:
    void slotSetIcon();

private:

    QHBoxLayout *m_menuLayout;
    QVBoxLayout *m_mainLayout;

    bool docWasModified;
    QString m_time;
    QString memo;

    void initForm();
    void initWidget();
    void initConnect();

    bool save();

public:
    void slotSave();
    void documentWasModified();
    void slotChoiceFont();
    void slotShowMemo();
    void setTextEdit(QString &string);
    void setCurrDate(QString &string);		//接入日期的slot
};

#endif // RIGHTDOWNWIDGET_H
