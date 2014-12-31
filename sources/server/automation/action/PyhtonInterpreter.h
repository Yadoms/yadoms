#pragma once
#include <shared/DataContainer.h>
#include "IScriptInterpreter.h"

namespace automation { namespace action
{
   //-----------------------------------------------------
   ///\brief The Python interpreter
   //-----------------------------------------------------
   class CPythonInterpreter : public IScriptInterpreter
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] scriptPath    Script path
      ///\param[in] configuration Script configuration
      //-----------------------------------------------------
      CPythonInterpreter(const std::string& scriptPath, const shared::CDataContainer& configuration);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CPythonInterpreter();

   protected:
      // IScriptInterpreter Implementation
      virtual void run(IYScriptApi& context) const;
      virtual void stop() const;
      virtual bool isOk() const;
      virtual std::string error() const;
      // [END] IAction IScriptInterpreter

   private:
      //-----------------------------------------------------
      ///\brief               The script path
      //-----------------------------------------------------
      const std::string m_scriptPath;
   };
	
} } // namespace automation::action
	
	