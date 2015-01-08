#pragma once
#include "IScriptRunnerFactory.h"
#include <shared/script/IScriptInterpreter.h>

namespace automation { namespace action
{
   //-----------------------------------------------------
   ///\brief The script runner factory
   //-----------------------------------------------------
   class CScriptRunnerFactory : public IScriptRunnerFactory
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] interpretersPath Path where are located interpreters
      //-----------------------------------------------------
      CScriptRunnerFactory(const std::string& interpretersPath);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CScriptRunnerFactory();

   protected:
      // IScriptRunnerFactory Implementation
      virtual boost::shared_ptr<shared::script::IScriptRunner> createScriptRunner(const std::string& scriptPath, const shared::CDataContainer& scriptConfiguration) const;
      // [END] IScriptRunnerFactory Implementation

      //-----------------------------------------------------
      ///\brief               Load the interpreters
      //-----------------------------------------------------
      void loadInterpreters();

      //-----------------------------------------------------
      ///\brief               Get the interpreter needed to run a script
      ///\param[in] scriptPath Script full path
      ///\return              The first interpreter found supporting this script
      ///\throw CScriptInterpreterNotFound No corresponding script interpreter was found
      //-----------------------------------------------------
      boost::shared_ptr<shared::script::IScriptInterpreter> getScriptInterpreter(const std::string& scriptPath) const;

   private:
      //-----------------------------------------------------
      ///\brief               Path where are located interpreters
      //-----------------------------------------------------
      const boost::filesystem::path m_interpretersPath;

      //-----------------------------------------------------
      ///\brief               List of loaded interpreters
      //-----------------------------------------------------
      std::vector<boost::shared_ptr<shared::script::IScriptInterpreter> > m_LoadedInterpreters;
   };
	
} } // namespace automation::action
	
	