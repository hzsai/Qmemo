#ifndef ICIBA_WORD_H
#define ICIBA_WORD_H

#include <QString>
#include <QDateTime>
#include<QJsonObject>

class IcibaWord
{
public:
    explicit IcibaWord();

    // 初始化
    void init();
    void parsedate(QString date);
    void day_transfer(int days);

    // 管理json读写
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    void read_file();
    void write_file();
    void print();
    void tomorrow();
    void yesterday();
    QString parsefilename() const;

    // getter & setter
    int get_year();
    int get_month();
    int get_day();
    QString get_picture2();
    QString get_content();

private:

    // 日期构造必须,在读取时构造
    int m_year;
    int m_month;
    int m_day;

    // 简化日期加减操作
    QDateTime m_datetime;

    // josn 数据
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

    // 控制读写
    bool is_read;
};

#endif // ICIBA_WORD_H
