#include "rightdownwidget.h"
#include "database/database.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QMessageBox>
#include <QDateTime>
#include <QtSql/QSqlQuery>
#include <QAction>
#include <QFont>
#include <QLabel>
#include <QFontDialog>
#include <QSettings>
#include <QDebug>
#include <QStyle>
#include <QCoreApplication>

RightDownWidget::RightDownWidget(QWidget * parent)
    :QWidget(parent)
{
    initWidget();
    initForm();
    initConnect();
}

RightDownWidget::~RightDownWidget()
{
}

void RightDownWidget::initForm()
{
    this->setFocusPolicy(Qt::ClickFocus);
    this->setObjectName(tr("rightdownwidget"));
    this->setFixedSize(620, 250);
    QString filepath = QCoreApplication::applicationDirPath() + "/qmemo.ini";
    QSettings setting(filepath, QSettings::IniFormat);
    setting.setIniCodec("utf-8");
    if (setting.contains("Qmemo/TextfontFamily")) {
        QFont f;
        QString fontFamily = setting.value("Qmemo/TextfontFamily").toString();
        qint64 fontpointSize = setting.value("Qmemo/TextpointSize").toInt();
        f.setFamily(fontFamily);
        f.setPointSize(fontpointSize);
        m_textEdit->setFont(f);
    }
}

void RightDownWidget::initWidget()
{
    //文本编辑 区
    m_textEdit = new QTextEdit(this);
    m_textEdit->setWordWrapMode(QTextOption::WordWrap);

    //底部按钮
    m_save = new QPushButton(this);
    m_save->setIcon(QIcon(":images/rightdownwiget/save.png"));
    m_save->setText(tr("保存"));

    //布局，总的是垂直布局，底部栏是水平布局
    m_menuLayout = new QHBoxLayout();
    m_menuLayout->addStretch();
    m_menuLayout->addWidget(m_save);
    m_menuLayout->addStretch();
    m_mainLayout = new QVBoxLayout();

    //总布局添加组件、布局
    m_mainLayout->addWidget(m_textEdit);
    m_mainLayout->addLayout(m_menuLayout);

    //总布局
    this->setLayout(m_mainLayout);

    docWasModified = false;
}

void RightDownWidget::initConnect()
{
    //信号槽连接，响应保存按钮，还可以继续添加功能,,,
    connect(m_save, &QPushButton::clicked,
            this, &RightDownWidget::slotSave);

    connect(m_textEdit->document(), &QTextDocument::contentsChanged,
            this, &RightDownWidget::documentWasModified);

    connect(m_save, &QPushButton::clicked,
            this, &RightDownWidget::slotSetIcon);
}

void RightDownWidget::slotSetIcon()
{
    memo = m_textEdit->document()->toPlainText();
    if (currDate.isEmpty())
        return ;
    int nday = currDate.split("-").at(2).toInt();

    // filter
    if (!memo.isEmpty())
        emit signalSetIcon(nday);
}

void RightDownWidget::documentWasModified()
{
    setWindowModified(true);
    m_textEdit->document()->setModified(true);
    docWasModified = true;
}

//更改后的保存询问,
bool RightDownWidget::maybeSave()
{
    if (!m_textEdit->document()->isModified())
        return false;
    QString strDate = currDate;
    QString infoText = QString("日期<b> %1 </b>有内容被修改了\n保存吗?").arg(strDate);

    QMessageBox messageBox;

    messageBox.setInformativeText(infoText);
    messageBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);

    messageBox.setButtonText(QMessageBox::Ok, "保存");
    messageBox.setButtonText(QMessageBox::Cancel, "取消");

    auto ret = messageBox.exec();
    switch (ret) {
    case QMessageBox::Ok:
        slotSave();
        return true;
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

//数据库保存
bool RightDownWidget::save()
{
    //database operation.
    //先清除那天的数据，再存储, replace into
    QString m_time = currDate;
    QSqlQuery query;

    //addBindValue()是添加绑定值, bindValue()是绑定某值到某值
    QString sql_insert = "replace into daily (note, time) values(:note, :time);";
    query.prepare(sql_insert);
    QString str = m_textEdit->toPlainText();
    query.bindValue(":note", str);
    query.bindValue(":time", m_time);
    query.exec();

    emit signalRightDownWidgetSave(str, currDate.split("-").at(2).toInt());
    return true;
}

//槽函数
void RightDownWidget::slotSave()
{
    // filter
    if (currDate.isEmpty())
        return ;
    if (save()) {
        setWindowModified(false);
        m_textEdit->document()->setModified(false);
        docWasModified = false;
        emit sig_dailyBoxMessage("保存成功!");
    } else {
        QMessageBox::warning(this, tr("提示"), tr("因未选择日期，未能保存成功，(。﹏。*)"));
        if (!m_textEdit->document()->toPlainText().isEmpty()) {
            setWindowModified(true);
            m_textEdit->document()->setModified(true);
            docWasModified = true;
        }
    }
    update();
}

void RightDownWidget::slotChoiceFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(
                &ok, QFont("Helverica [Cronyx]"), this);
    if (ok) {
        QMessageBox::information(this, tr("En"),
                                 tr("Ok!"));
    } else {
        QMessageBox::information(this, tr("En"),
                                 tr("Never Mind"));
    }

    m_textEdit->setFont(font);

    QString filepath = QCoreApplication::applicationDirPath() + "/qmemo.ini";
    QSettings setting(filepath, QSettings::IniFormat);
    setting.setIniCodec("utf-8");
    setting.setValue("Qmemo/TextFont", font.key());
    setting.setValue("Qmemo/TextfontFamily", font.family());
    setting.setValue("Qmemo/TextpointSize", font.pointSize());
    setting.setValue("Qmemo/TextStyle", font.style());
    // family, pixelSize, pointSize, styleHint, styleName;
}

void RightDownWidget::slotShowMemo()
{
}

void RightDownWidget::setTextEdit(QString &string)
{
    bool saved = maybeSave();

    if (!saved)
    {
    }

    m_textEdit->setText(string);

    update();
}

void RightDownWidget::setCurrDate(QString &string)
{
    prevDate = currDate;
    currDate = string;
}
