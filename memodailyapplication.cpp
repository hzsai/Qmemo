#include "memodailyapplication.h"
#include <QDebug>

//单实例
MemoDailyApplication::MemoDailyApplication(int &argc, char *argv[])
    :QApplication(argc, argv)
{
    m_sharedMemory.setKey("Memo");
    if (m_sharedMemory.attach()) {
        m_IsRun = true;
    } else {
        m_IsRun = false;
        if (!m_sharedMemory.create(1)) {
            qDebug()<<"Unable to create single instance !";
            qDebug()<<m_sharedMemory.errorString();
            return ;
        }
    }
}

MemoDailyApplication::~MemoDailyApplication()
{

}

bool MemoDailyApplication::isRunning()
{
    return m_IsRun;
}
