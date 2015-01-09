#pragma once
#include "IScriptInterpreterLibrary.h"
#include <shared/DynamicLibrary.h>

namespace automation { namespace action
{
   //-----------------------------------------------------
   ///\brief The script interpreter library
   //-----------------------------------------------------
   class CScriptInterpreterLibrary : public IScriptInterpreterLibrary
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] interpreterPath Interpreter path
      ///\throw CInvalidParameter if unable to load interpreter
      //-----------------------------------------------------
      CScriptInterpreterLibrary(const boost::filesystem::path& interpreterPath);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CScriptInterpreterLibrary();

   protected:
      // IScriptInterpreterLibrary Implementation
      virtual boost::shared_ptr<shared::script::IScriptInterpreter> getInterpreter() const;
      // [END] IScriptInterpreterLibrary Implementation
      
      //-----------------------------------------------------
      ///\brief               Load the library and create interpreter
      //-----------------------------------------------------
      boost::shared_ptr<shared::script::IScriptInterpreter> load(const boost::filesystem::path& interpreterPath);

   private:
      //-----------------------------------------------------
      ///\brief               The loaded library
      //-----------------------------------------------------
      boost::shared_ptr<shared::CDynamicLibrary> m_library;

      //-----------------------------------------------------
      ///\brief               The created interpreter
      //-----------------------------------------------------
      boost::shared_ptr<shared::script::IScriptInterpreter> m_interpreter;
   };
	
} } // namespace automation::action
	
	