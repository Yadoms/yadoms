#include "stdafx.h"
#include "Executable.h"

namespace shared
{
   std::string CExecutable::Extension() { return "exe"; }
   std::string CExecutable::DotExtension() { return ".exe"; }

   std::string CExecutable::ToFileName(const std::string& exeName)
   {
      return exeName + DotExtension();
   }

   std::string CExecutable::ToExeName(const std::string& fileName)
   {
      boost::filesystem::path exeFile(fileName);
      return exeFile.stem().string();
   }

} // namespace shared