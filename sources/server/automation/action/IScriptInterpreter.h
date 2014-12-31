#pragma once
#include "IYScriptApi.h"

namespace automation { namespace action
{
   //-----------------------------------------------------
   ///\brief The script interpreter interface
   //-----------------------------------------------------
   class IScriptInterpreter
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IScriptInterpreter() {}

      //-----------------------------------------------------
      ///\brief               Run the script (blocks while script is running)
      ///\param[in] context   IYScriptApi context, use by the script to interact with Yadoms
      //-----------------------------------------------------
      virtual void run(IYScriptApi& context) const = 0;

      //-----------------------------------------------------
      ///\brief               Stop a runnning script
      //-----------------------------------------------------
      virtual void stop() const = 0;

      //-----------------------------------------------------
      ///\brief               Check if last script execution was OK
      ///\return              Return the script result (true if no error)
      //-----------------------------------------------------
      virtual bool isOk() const = 0;

      //-----------------------------------------------------
      ///\brief               Get the error message
      ///\return              Return the error message (EmptyString if isOk is true)
      //-----------------------------------------------------
      virtual std::string error() const = 0;
   };
	
} } // namespace automation::action	
	
	