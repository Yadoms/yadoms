#pragma once
#include "IScriptInterpreterFactory.h"

namespace automation { namespace action
{
   //-----------------------------------------------------
   ///\brief The script interpreter factory
   //-----------------------------------------------------
   class CScriptInterpreterFactory : public IScriptInterpreterFactory
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] configuration Action configuration
      //-----------------------------------------------------
      CScriptInterpreterFactory();

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CScriptInterpreterFactory();

   protected:
      // IScriptInterpreterFactory Implementation
      virtual boost::shared_ptr<IScriptInterpreter> createScriptInterpreter(const std::string& scriptPath, const shared::CDataContainer& scriptConfiguration) const;
      // [END] IAction Implementation
   };
	
} } // namespace automation::action
	
	