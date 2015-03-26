#include "stdafx.h"
#include "ErrorHandler.h"
#include <Poco/Thread.h>
#include <shared/Log.h>

void CErrorHandler::exception(const Poco::Exception& exc)
{
   YADOMS_LOG(error) << "Global catch Poco::Exception : " << exc.displayText() << std::endl << "    in thread : " << Poco::Thread::current()->getName() << std::endl;
}

void CErrorHandler::exception(const std::exception& exc)
{
   YADOMS_LOG(error) << "Global catch std::Exception : " << exc.what() << std::endl << "    in thread : " << Poco::Thread::current()->getName() << std::endl;
}

void CErrorHandler::exception()
{
   YADOMS_LOG(error) << "Global catch ... : " << "unknown exception" << std::endl << "    in thread : " << Poco::Thread::current()->getName() << std::endl;
}
