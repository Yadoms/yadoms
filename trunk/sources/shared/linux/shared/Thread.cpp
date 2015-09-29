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
   // I don't know how to kill a thread without killing the process,
   // so prefer to kill nothing !
}

} // namespace shared