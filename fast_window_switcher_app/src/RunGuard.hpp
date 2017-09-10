/***************************************************************************
** This file is part of the FastWindowSwitcher

//From:
//https://stackoverflow.com/questions/5006547/qt-best-practice-for-a-single-instance-app-protection

******************************************************************************/

#include <QObject>
#include <QSharedMemory>
#include <QSystemSemaphore>

namespace FastWindowSwitcher
{
  class RunGuard
  {
  public:
    explicit RunGuard(const QString& key);
    ~RunGuard();

    bool IsAnotherRunning();
    bool TryToRun();
    void Release();

  private:
    const QString key;
    const QString memLockKey;
    const QString sharedmemKey;
    QSharedMemory sharedMem;
    QSystemSemaphore memLock;

    Q_DISABLE_COPY(RunGuard)
  };
};