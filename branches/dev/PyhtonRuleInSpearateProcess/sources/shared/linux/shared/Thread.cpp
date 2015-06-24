#include "stdafx.h"
#include "Thread.h"
#include <pthread.h>
#include <signal.h>

namespace shared
{

CThread::CThread()
{
}

CThread::~CThread()
{
}

void CThread::killThread(pthread_t hThread)
{
   pthread_kill(hThread, 9);
}

} // namespace shared