#include "stdafx.h"
#include "Executable.h"

namespace shared
{

   std::string CExecutable::Extension() { return ""; }
   std::string CExecutable::DotExtension() { return ""; }

   std::string CExecutable::ToFileName(const std::string& exeName)
   {
      return exeName;
   }

   std::string CExecutable::ToExeName(const std::string& fileName)
   {
      return fileName;
   }

} // namespace shared