#ifndef RIGHTDOWNWIDGET_H
#define RIGHTDOWNWIDGET_H

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

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
    QTextEdit *m_textEdit{nullptr};
    bool maybeSave();
    QPushButton *m_save{nullptr};
    QString currDate;			//用于外部接入数 据。
    QString prevDate;

signals:
    void signalSetIcon(int );
    void signalRightDownWidgetSave(QString &, int);
    void sig_dailyBoxMessage(QString str);

public slots:
    void slotSetIcon();

private:

    QHBoxLayout *m_menuLayout{nullptr};
    QVBoxLayout *m_mainLayout{nullptr};

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
