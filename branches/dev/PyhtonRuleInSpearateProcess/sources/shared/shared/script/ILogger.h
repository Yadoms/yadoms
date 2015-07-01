#pragma once

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

      //-----------------------------------------------------
      ///\brief               Get the out log stream
      ///\return              The out log stream
      //-----------------------------------------------------
      virtual std::ostream& out() = 0;

      //-----------------------------------------------------
      ///\brief               Get the error log stream
      ///\return              The error log stream
      //-----------------------------------------------------
      virtual std::ostream& error() = 0;
   };

} } // namespace shared::script

