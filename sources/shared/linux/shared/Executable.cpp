#include "stdafx.h"
#include "Executable.h"

namespace shared
{

   const std::string CExecutable::Extension() { return ""; }
   const std::string CExecutable::DotExtension() { return ""; }

   const std::string CExecutable::ToFileName(const std::string& exeName)
   {
      return exeName;
   }

   const std::string CExecutable::ToExeName(const std::string& fileName)
   {
      return fileName;
   }

} // namespace shared