#pragma once
#include "IScriptInterpreter.h"
#include <shared/shared/DataContainer.h>

namespace automation { namespace action
{
   //-----------------------------------------------------
   ///\brief The script interpreter factory interface
   //-----------------------------------------------------
   class IScriptInterpreterFactory
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IScriptInterpreterFactory() {}

      //-----------------------------------------------------
      ///\brief               Create a script interpreter
      ///\param[in] scriptPath    Script path
      ///\param[in] configuration Script configuration
      ///\return              The script interpreter instance
      //-----------------------------------------------------
      virtual boost::shared_ptr<IScriptInterpreter> createScriptInterpreter(const std::string& scriptPath, const shared::CDataContainer& scriptConfiguration) const = 0;
   };
	
} } // namespace automation::action	
	
	