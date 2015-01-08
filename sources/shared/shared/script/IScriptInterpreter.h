#pragma once
#include "IScriptRunner.h"

namespace shared { namespace script
{
   //-----------------------------------------------------
   ///\brief The script interpreter interface
   //-----------------------------------------------------
   class IScriptInterpreter
   {
   public:
      //-----------------------------------------------------
      ///\brief                  Destructor
      //-----------------------------------------------------
      virtual ~IScriptInterpreter() {}

      //-----------------------------------------------------
      ///\brief                  Check if the interpreter is fully functionnal
      ///\return                 true if the interpreter is fully functionnal
      ///\note This method is usefull when interpreter depends on libraries or services, that
      /// are optional on system (for example : Python interpreter). It checks that all the chain
      /// is ready to interpret a script file.
      //-----------------------------------------------------
      virtual bool isAvailable() const = 0;

      //-----------------------------------------------------
      ///\brief                  Check if the interpreter can interpret a script file
      ///\param[in] scriptPath   Full path of script file
      ///\return                 true if the script file is supported
      //-----------------------------------------------------
      virtual bool canInterpret(const std::string& scriptPath) const = 0;

      //-----------------------------------------------------
      ///\brief                  Create a script runner with associated file
      ///\param[in] scriptPath   Full path of script file
      ///\return                 The created script instance
      //-----------------------------------------------------
      virtual boost::shared_ptr<IScriptRunner> createRunner(const std::string& scriptPath) const = 0;
   };
	
} } // namespace shared::script
	
	