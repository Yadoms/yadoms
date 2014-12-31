#include "stdafx.h"
#include "PyhtonInterpreter.h"
#include <shared/StringExtension.h>

namespace automation { namespace action
{
   //TODO all here is to do
CPythonInterpreter::CPythonInterpreter(const std::string& scriptPath, const shared::CDataContainer& configuration)
   :m_scriptPath(scriptPath)
{
}

CPythonInterpreter::~CPythonInterpreter()
{         
}

void CPythonInterpreter::run(IYScriptApi& context) const
{
   
}

void CPythonInterpreter::stop() const
{
   
}

bool CPythonInterpreter::isOk() const
{
   return true;
}

std::string CPythonInterpreter::error() const
{
   return shared::CStringExtension::EmptyString;
}

} } // namespace automation::action	
	
	