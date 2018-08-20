#ifndef ICIBA_WORD_H
#define ICIBA_WORD_H

#include <QString>
#include <QDateTime>
#include<QJsonObject>

class IcibaWord
{
public:
    explicit IcibaWord();

    QString caption;
    QString content;
    QString dateline;
    QString fenxiang_img;
    int love;
    QString note;
    QString picture;
    QString picture2;
    int s_pv;
    int sid;
    int sp_pv;
    QString translation;
    QString tts;


    // 管理json读写
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    void read_file();
    void write_file();
    void print();
};

#endif // ICIBA_WORD_H
