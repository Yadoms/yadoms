#pragma once
#include <Poco/LogStream.h>

namespace shared { namespace process
{
   //-----------------------------------------------------
   ///\brief The process logger interface
   //-----------------------------------------------------
   class ILogger
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~ILogger() {}

      //-----------------------------------------------------
      ///\brief               Log the message
      ///\param[in] msg       Message to log
      //-----------------------------------------------------
      virtual void log(const std::string& msg) = 0;
   };

} } // namespace shared::process

