#include "memowidget.h"
#include "ui/MidRightUpWidget/rightupwidget.h"
#include "ui/TitleBar/my_menu.h"
#include <QWidget>
#include <QFile>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QColor>
#include <QSettings>

RightUpWidget::RightUpWidget(QWidget *parent):QWidget(parent)
{
    this->setFixedSize(620, 350);
    initForm();
    initConnect();
}
RightUpWidget::~RightUpWidget()
{

}

void RightUpWidget::initConnect()
{
    connect(this, &RightUpWidget::signalSetPicPath,
            this, &RightUpWidget::slotSetPicPath);
}

void RightUpWidget::initForm()
{
    // 上一张、下一张按钮
    labelprev = new QPushButton(this);
    labelnext = new QPushButton(this);

    labelprev->setText("<<");
    labelnext->setText(">>");
    labelprev->setGeometry(20, 165, 20, 20);
    labelnext->setGeometry(580, 165, 20, 20);
    labelnext->setVisible(false);
    labelprev->setVisible(false);

    // 图片
    this->picPath = "";
    QSettings setting("./qmemo.ini", QSettings::IniFormat);
    if (setting.contains("Qmemo/PicturePath")) {
        QString pth = setting.value("Qmemo/PicturePath").toString();
        this->picPath = pth;
    } else {
        this->picPath = ":/images/20170420031214123.jpg";
        setting.setValue("Qmemo/PicturePath", picPath);
    }
    loadImage(picPath);
}

void RightUpWidget::slotLoadImage()
{
    loadImage(picPath);
}

void RightUpWidget::slotLoadImage(QString file_path)
{
    loadImage(file_path);
}

void RightUpWidget::loadImage(QString fileName)
{
    QPixmap pixmap;
    pixmap.load(fileName);
    m_currPixmap = pixmap;
    // refresh
    repaint();
}

void RightUpWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    drawBackground(&painter);
    drawImage(&painter);
    showSuitableSize(&painter);
}

void RightUpWidget::drawBackground(QPainter *p)
{
    p->save();
    // QColor
    QLinearGradient lineargradient(QPointF(0, 0), QPointF(0, height()));
    lineargradient.setColorAt(0.0, QColor(100, 100, 100));
    lineargradient.setColorAt(1.0,  QColor(60, 60, 60));
    p->setPen(Qt::NoPen);
    p->setBrush(lineargradient);
    p->drawRect(rect());

    p->restore();
}

void RightUpWidget::drawImage(QPainter *p)
{
    if (m_currPixmap.isNull()) return ;
    showSuitableSize(p);
}

void RightUpWidget::showSuitableSize(QPainter *p)
{
    p->save();

    // Suitabe size
    //int pixWidth = m_currPixmap.width();
    //int pixHeight = m_currPixmap.height();
    //qDebug() << pixHeight << " " << pixWidth;
    //qreal wRatio = 1.0 * width() / m_currPixmap.width();
    //qreal hRatio = 1.0 * height() / m_currPixmap.height();
    //qreal ratio = qMin(wRatio, hRatio);
    //pixWidth *= ratio;
    //pixHeight *= ratio;
    //int x = rect().center().x()-pixWidth / 2;
    //int y = rect().center().y()-pixHeight / 2;
    //p->drawPixmap(x, y, pixWidth, pixHeight, m_currPixmap);

    // Fill size
    p->drawPixmap(rect(), m_currPixmap);
    p->restore();
}

void RightUpWidget::slotSetPicPath()
{
    QSettings setting("./qmemo.ini", QSettings::IniFormat);
    // 接入菜单栏的选择路径设置path
    QString path = QFileDialog::getOpenFileName(this, tr("选择图片"),
                                                tr("."));
    if (path.isEmpty()) {
        QMessageBox::warning(this, tr("提示"),
                             tr("打开图片出错！"));
        return ;
    }
    picPath = path;
    setting.setValue("Qmemo/PicturePath", path);
    loadImage(path);

    repaint();
    update();
}


