#pragma once
#include <shared/script/IScriptInterpreter.h>

namespace automation { namespace action
{
   //-----------------------------------------------------
   ///\brief The script interpreter library interface
   //-----------------------------------------------------
   class IScriptInterpreterLibrary
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IScriptInterpreterLibrary() {}

      //-----------------------------------------------------
      ///\brief               Get the loaded script interpreter
      ///\param[in] interpreterPath    Interpreter path
      ///\return              The loaded script interpreter
      //-----------------------------------------------------
      virtual boost::shared_ptr<shared::script::IScriptInterpreter> getInterpreter() const = 0;
   };
	
} } // namespace automation::action	
	
	