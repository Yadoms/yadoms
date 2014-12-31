#include "stdafx.h"
#include "ScriptInterpreterFactory.h"
#include "PyhtonInterpreter.h"
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/exception/OutOfRange.hpp>
#include "ScriptInterpreterType.h"

namespace automation { namespace action
{

CScriptInterpreterFactory::CScriptInterpreterFactory()
{
}

CScriptInterpreterFactory::~CScriptInterpreterFactory()
{         
}

boost::shared_ptr<IScriptInterpreter> CScriptInterpreterFactory::createScriptInterpreter(
   const std::string& scriptPath, const shared::CDataContainer& scriptConfiguration) const
{
   boost::shared_ptr<IScriptInterpreter> interpreter;

   try
   {
      std::string scriptExtension(boost::filesystem::path(scriptPath).extension().string());
      EScriptType type(scriptExtension);

      switch (type)
      {
      case EScriptType::kPythonValue: interpreter.reset(new CPythonInterpreter(scriptPath, scriptConfiguration)); break;
      default: throw shared::exception::COutOfRange("type value exists but is not yet implemented");
      }
   }
   catch (shared::exception::COutOfRange& e)
   {
      YADOMS_LOG(error) << "Script type is not supported : " << e.what();
      throw shared::exception::CInvalidParameter("script type");
   }

   return interpreter;
}

} } // namespace automation::action	
	
	