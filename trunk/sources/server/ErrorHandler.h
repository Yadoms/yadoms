#pragma once
#include <Poco/ErrorHandler.h>

//-----------------------------
///\brief SdBs2 global error handler (catch all unhandled exception in Poco application)
//-----------------------------
class CErrorHandler : public Poco::ErrorHandler
{
public:
   void exception(const Poco::Exception& exc);
   void exception(const std::exception& exc);
   void exception();
};