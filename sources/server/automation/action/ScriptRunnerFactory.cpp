#include "stdafx.h"
#include "ScriptRunnerFactory.h"
#include "ScriptInterpreterNotFound.hpp"
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/exception/OutOfRange.hpp>
#include <shared/script/IScriptInterpreter.h>

namespace automation { namespace action
{

CScriptRunnerFactory::CScriptRunnerFactory(const std::string& interpretersPath)
   :m_interpretersPath(interpretersPath)
{
   loadInterpreters();
}

CScriptRunnerFactory::~CScriptRunnerFactory()
{         
}

void CScriptRunnerFactory::loadInterpreters()
{
   //TODO créer la liste
}

boost::shared_ptr<shared::script::IScriptRunner> CScriptRunnerFactory::createScriptRunner(
   const std::string& scriptPath, const shared::CDataContainer& scriptConfiguration) const
{
   try
   {
      boost::shared_ptr<shared::script::IScriptInterpreter> scriptInterpreter = getScriptInterpreter(scriptPath);

      return scriptInterpreter->createRunner(scriptPath);
   }
   catch (CScriptInterpreterNotFound& e)
   {
      YADOMS_LOG(error) << "Unable to run script : " << e.what();
      throw shared::exception::CInvalidParameter("Unable to run script");
   }
   catch (shared::exception::COutOfRange& e)
   {
      YADOMS_LOG(error) << "Script type is not supported : " << e.what();
      throw shared::exception::CInvalidParameter("script type");
   }
}

boost::shared_ptr<shared::script::IScriptInterpreter> CScriptRunnerFactory::getScriptInterpreter(const std::string& scriptPath) const
{
   for (std::vector<boost::shared_ptr<shared::script::IScriptInterpreter> >::const_iterator interpreter = m_LoadedInterpreters.begin(); interpreter != m_LoadedInterpreters.end(); ++interpreter)
   {
      if ((*interpreter)->canInterpret(scriptPath) && (*interpreter)->isAvailable()) //TODO : tester isAvailable au moment de la construction de la liste m_LoadedInterpreters ?
         return *interpreter;
   }

   throw CScriptInterpreterNotFound(scriptPath);
}

} } // namespace automation::action	
	
	