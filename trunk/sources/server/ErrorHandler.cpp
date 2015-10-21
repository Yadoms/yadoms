#include "stdafx.h"
#include "ErrorHandler.h"
#include <Poco/Thread.h>
#include <shared/Log.h>
#include <shared/tools/Debug.h>

void CErrorHandler::exception(const Poco::Exception& exc)
{
   //log thread ID
   shared::tools::CDebug::LogThreadId("GlobalCatch");

   Poco::Thread * currentThread = Poco::Thread::current();
   if (currentThread != NULL)
   {
      YADOMS_LOG(error) << "Global catch Poco::Exception : " << exc.displayText() << std::endl << "    in thread : " << Poco::Thread::current()->getName() << std::endl;
   }
   else
   {
      YADOMS_LOG(error) << "Global catch Poco::Exception : " << exc.displayText();
   }
}

void CErrorHandler::exception(const std::exception& exc)
{
   //log thread ID
   shared::tools::CDebug::LogThreadId("GlobalCatch");

   Poco::Thread * currentThread = Poco::Thread::current();
   if (currentThread != NULL)
   {
      YADOMS_LOG(error) << "Global catch std::Exception : " << exc.what() << std::endl << "    in thread : " << Poco::Thread::current()->getName() << std::endl;
   }
   else
   {
      YADOMS_LOG(error) << "Global catch std::Exception : " << exc.what();
   }
}

void CErrorHandler::exception()
{
   //log thread ID
   shared::tools::CDebug::LogThreadId("GlobalCatch");

   Poco::Thread * currentThread = Poco::Thread::current();
   if (currentThread != NULL)
   {
      YADOMS_LOG(error) << "Global catch ... : " << "unknown exception" << std::endl << "    in thread : " << currentThread->getName() << std::endl;
   }
   else
   {
      YADOMS_LOG(error) << "Global catch ... : " << "unknown exception";
   }
}
