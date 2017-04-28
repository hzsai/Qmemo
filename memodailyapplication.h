#ifndef MEMODAILYAPPLICATION_H
#define MEMODAILYAPPLICATION_H
#include <QApplication>
#include <QSharedMemory>

class MemoDailyApplication : public QApplication
{
public:
    MemoDailyApplication(int &argc, char *argv[]);
    ~MemoDailyApplication();
    bool isRunning();
private:
    bool m_IsRun;
    QSharedMemory m_sharedMemory;
};

#endif // MEMODAILYAPPLICATION_H
