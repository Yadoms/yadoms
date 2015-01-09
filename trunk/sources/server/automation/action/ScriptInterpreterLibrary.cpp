#include "stdafx.h"
#include "ScriptInterpreterLibrary.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/script/IScriptInterpreter.h>

namespace automation { namespace action
{

CScriptInterpreterLibrary::CScriptInterpreterLibrary(const boost::filesystem::path& interpreterPath)
{
   load(interpreterPath);
}

CScriptInterpreterLibrary::~CScriptInterpreterLibrary()
{
}

boost::shared_ptr<shared::script::IScriptInterpreter> CScriptInterpreterLibrary::load(const boost::filesystem::path& interpreterPath)
{
   try
   {
      // Load library
      m_library.reset(new shared::CDynamicLibrary(interpreterPath.string()));
   }
   catch (shared::exception::CInvalidParameter&)
   {
      throw shared::exception::CInvalidParameter("Unable to load library");
   }

   // Get entry point
   boost::function<shared::script::IScriptInterpreter* ()> m_construct = (shared::script::IScriptInterpreter* (*)(void))(m_library->GetFunctionPointer("construct"));
   if (m_construct == NULL)
      throw shared::exception::CInvalidParameter("Unable to get entry point");

   // Create interpreter object
   m_interpreter.reset(m_construct());
   if (!m_interpreter)
      throw shared::exception::CInvalidParameter("Unable to create interpreter object");

   // Check if all interpreter toolchain is in order
   if (!m_interpreter->isAvailable())
      throw shared::exception::CInvalidParameter("interpreter toolchain is not ready");

   return m_interpreter;
}

boost::shared_ptr<shared::script::IScriptInterpreter> CScriptInterpreterLibrary::getInterpreter() const
{
   return m_interpreter;
}

} } // namespace automation::action	
	
	