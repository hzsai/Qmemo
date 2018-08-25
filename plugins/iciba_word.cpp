#include "iciba_word.h"
#include <QDateTime>
#include <QDate>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>
#include <QStringList>

IcibaWord::IcibaWord():
    caption(""),
    content(""),
    dateline(""),
    fenxiang_img(""),
    love(0),
    note(""),
    picture(""),
    picture2(""),
    s_pv(0),
    sid(1),
    sp_pv(0),
    translation(""),
    tts(""),
    is_read(false)
{
    init();
}

void IcibaWord::init()
{
    m_year = QDateTime::currentDateTime().date().year();
    m_month = QDateTime::currentDateTime().date().month();
    m_day = QDateTime::currentDateTime().date().day();
    m_datetime.setDate(QDate(m_year, m_month, m_day));
}

QString IcibaWord::get_picture2()
{
    return picture2;
}

int IcibaWord::get_year()
{
    return m_year;
}

int IcibaWord::get_month()
{
    return m_month;
}

int IcibaWord::get_day()
{
    return m_day;
}

void IcibaWord::read(const QJsonObject &json)
{
    caption = json["caption"].toString();
    content = json["content"].toString();
    dateline = json["dateline"].toString();
    fenxiang_img = json["fenxiang_img"].toString();
    love = json["love"].toString().toInt();
    note = json["note"].toString();
    picture = json["picture"].toString();
    picture2 = json["picture2"].toString();
    s_pv = json["s_pv"].toString().toInt();
    sid = json["sid"].toString().toInt();
    sp_pv = json["sp_pv"].toString().toInt();
    translation = json["translation"].toString();
    tts = json["tts"].toString();

    parsedate(dateline);
}

void IcibaWord::read_file()
{
    QFile in_file("today_weather.json");

    if (!in_file.open(QIODevice::ReadOnly)) {
        qWarning("read, open file failed.");
        return ;
    }

    QByteArray loaddata = in_file.readAll();
    QJsonDocument jsondoc(QJsonDocument::fromJson(loaddata));
    read(jsondoc.object());
    is_read = true;
    print();
}

void IcibaWord::write(QJsonObject &json) const
{
    json["caption"] 		= caption;
    json["content"] 		= content;
    json["dateline"] 		= dateline;
    json["fenxiang_img"]	= fenxiang_img;
    json["love"] 			= love;
    json["note"] 			= note;
    json["picture"] 		= picture;
    json["picture2"] 		= picture2;
    json["s_pv"] 			= s_pv;
    json["sid"] 			= sid;
    json["sp_pv"] 			= sp_pv;
    json["translation"] 	= translation;
    json["tts"] 			= tts;
}

void IcibaWord::write_file()
{
    QFile out_file("today_weather.json");

    if (!out_file.open(QIODevice::WriteOnly)) {
        qWarning("write, open file failed.");
        return ;
    }

    QJsonObject outobj;
    write(outobj);
    QJsonDocument outdoc(outobj);
    out_file.write(outdoc.toJson());

    qDebug() << "写入成功";
}

void IcibaWord::print()
{
    qDebug() << "Caption: " 		<< caption;
    qDebug() << "Content: " 		<< content;
    qDebug() << "Dateline: " 		<< dateline;
    qDebug() << "Fenxiang_img: " 	<< fenxiang_img;
    qDebug() << "Love: " 			<< love;
    qDebug() << "Note: " 			<< note;
    qDebug() << "Picture: " 		<< picture;
    qDebug() << "Picture2: " 		<< picture2;
    qDebug() << "Sid: " 			<< sid;
    qDebug() << "Translation: " 	<< translation;
    qDebug() << "TTS: "				<< tts;
}

void IcibaWord::parsedate(QString date)
{
    QStringList lst = date.split("-");

    m_year = lst.at(0).toInt();
    m_month = lst.at(1).toInt();
    m_day = lst.at(2).toInt();
}

QString IcibaWord::parsefilename() const
{
    if (picture2 == nullptr)
        return "";
    QStringList lis = picture2.split("/");
    int length = lis.length();
    return lis.at(length-1);
}

void IcibaWord::tomorrow()
{
    if (!is_read)
        return ;
    day_transfer(1);
    qDebug("Day to tomorrow, please update info.");
}

void IcibaWord::yesterday()
{
    if (!is_read)
        return ;
    day_transfer(-1);
    qDebug("Day to yesterday, please update info.");
}

void IcibaWord::day_transfer(int days)
{
    m_datetime.setDate(QDate(m_year, m_month, m_day));
    m_datetime.addDays(days);
    m_year = m_datetime.currentDateTime().date().year();
    m_month = m_datetime.currentDateTime().date().month();
    m_day = m_datetime.currentDateTime().date().day();
}
