#include "iciba_word.h"
#include <QDateTime>
#include <QDate>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>

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
    tts("")
{
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
