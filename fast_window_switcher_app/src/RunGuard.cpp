/***************************************************************************
** This file is part of the FastWindowSwitcher
*From:
*https://stackoverflow.com/questions/5006547/qt-best-practice-for-a-single-instance-app-protection
*
******************************************************************************/

#include <QCryptographicHash>

#include "RunGuard.hpp"

namespace FastWindowSwitcher
{
  QString generateKeyHash(const QString& key, const QString& salt)
  {
    QByteArray data;

    data.append(key.toUtf8());
    data.append(salt.toUtf8());
    data = QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();

    return data;
  }

  RunGuard::RunGuard(const QString& key)
    : key(key)
    , memLockKey(generateKeyHash(key, "_memLockKey"))
    , sharedmemKey(generateKeyHash(key, "_sharedmemKey"))
    , sharedMem(sharedmemKey)
    , memLock(memLockKey, 1)
  {
    memLock.acquire();
    {
      QSharedMemory fix(sharedmemKey);    // Fix for *nix: http://habrahabr.ru/post/173281/
      fix.attach();
    }
    memLock.release();
  }

  RunGuard::~RunGuard()
  {
    Release();
  }

  bool RunGuard::IsAnotherRunning()
  {
    if (sharedMem.isAttached())
      return false;

    memLock.acquire();
    const bool isRunning = sharedMem.attach();
    if (isRunning)
      sharedMem.detach();
    memLock.release();

    return isRunning;
  }

  bool RunGuard::TryToRun()
  {
    if (IsAnotherRunning())   // Extra check
      return false;

    memLock.acquire();
    const bool result = sharedMem.create(sizeof(quint64));
    memLock.release();
    if (!result)
    {
      Release();
      return false;
    }

    return true;
  }

  void RunGuard::Release()
  {
    memLock.acquire();
    if (sharedMem.isAttached())
      sharedMem.detach();
    memLock.release();
  }
}
