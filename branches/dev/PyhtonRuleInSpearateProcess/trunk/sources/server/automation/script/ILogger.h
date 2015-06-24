#pragma once

namespace automation { namespace script
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

      //-----------------------------------------------------
      ///\brief               Log at information level
      ///\param[in] message   Message to log
      //-----------------------------------------------------
      virtual void logInformation(const std::string& message) = 0;

      //-----------------------------------------------------
      ///\brief               Log at error level
      ///\param[in] message   Message to log
      //-----------------------------------------------------
      virtual void logError(const std::string& message) = 0;
   };

} } // namespace automation::script

