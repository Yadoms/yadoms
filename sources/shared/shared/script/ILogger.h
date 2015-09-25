#pragma once
#include <Poco/LogStream.h>

namespace shared { namespace script
{
   //-----------------------------------------------------
   ///\brief The rule logger interface
   //-----------------------------------------------------
   class ILogger
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~ILogger() {}

      ////-----------------------------------------------------
      /////\brief               Get the out log stream
      /////\return              The out log stream
      ////-----------------------------------------------------
      //virtual Poco::LogStream& out() = 0;//TODO virer

      ////-----------------------------------------------------
      /////\brief               Get the error log stream
      /////\return              The error log stream
      ////-----------------------------------------------------
      //virtual Poco::LogStream& error() = 0;//TODO virer

      //-----------------------------------------------------
      ///\brief               Log the message
      ///\param[in] msg       Message to log
      //-----------------------------------------------------
      virtual void log(const std::string& msg) = 0;
   };

} } // namespace shared::script

