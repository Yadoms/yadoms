#pragma once
#include "yScriptApi/IYScriptApi.h"

namespace shared { namespace script
{
   //-----------------------------------------------------
   ///\brief The script runner interface
   //-----------------------------------------------------
   class IRunner
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IRunner() {}

      //-----------------------------------------------------
      ///\brief               Run the script (blocks while script is running)
      ///\param[in] context   IYScriptApi context, use by the script to interact with Yadoms
      //-----------------------------------------------------
      virtual void run(yScriptApi::IYScriptApi& context) const = 0;

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
	
} } // namespace shared::script
	
	