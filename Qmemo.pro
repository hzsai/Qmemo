#-------------------------------------------------
#
# Project created by QtCreator 2017-04-07T23:33:54
#
#-------------------------------------------------

QT       += core gui sql multimedia network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qmemo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    memowidget.cpp \
    ui/TitleBar/titlewidget.cpp \
    ui/TitleBar/my_menu.cpp \
    ui/Common/menu.cpp \
    ui/Common/pushbutton.cpp \
    ui/MidRightUpWidget/rightupwidget.cpp \
    ui/MidRightDownWidget/rightdownwidget.cpp \
    database/database.cpp \
    ui/MidLeftWidget/datewidget.cpp \
    ui/MidLeftWidget/date.cpp \
    plugins/iciba_word.cpp \
    plugins/qreplytimeout.cpp

HEADERS  += \
    memowidget.h \
    ui/TitleBar/titlewidget.h \
    ui/TitleBar/my_menu.h \
    ui/Common/menu.h \
    ui/Common/pushbutton.h \
    ui/MidRightUpWidget/rightupwidget.h \
    ui/MidRightDownWidget/rightdownwidget.h \
    database/database.h \
    ui/MidLeftWidget/datewidget.h \
    ui/MidLeftWidget/date.h \
    plugins/iciba_word.h \
    plugins/qreplytimeout.h

RESOURCES += \
    res.qrc

FORMS +=

CONFIG += c++11

DISTFILES += \
    extra/note.txt

RC_ICONS = images/note_128px.ico

CONFIG(debug) {
        DEFINES += KK
}
