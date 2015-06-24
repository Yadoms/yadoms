#include "stdafx.h"
#include "InterpreterLibrary.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/script/IInterpreter.h>

namespace automation { namespace action { namespace script
{

CInterpreterLibrary::CInterpreterLibrary(const boost::filesystem::path& interpreterPath)
{
   load(interpreterPath);
}

CInterpreterLibrary::~CInterpreterLibrary()
{
}

boost::shared_ptr<shared::script::IInterpreter> CInterpreterLibrary::load(const boost::filesystem::path& interpreterPath)
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
   boost::function<shared::script::IInterpreter* ()> m_construct = (shared::script::IInterpreter* (*)(void))(m_library->GetFunctionPointer("construct"));
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

boost::shared_ptr<shared::script::IInterpreter> CInterpreterLibrary::getInterpreter() const
{
   return m_interpreter;
}

} } } // namespace automation::action::script
	
	