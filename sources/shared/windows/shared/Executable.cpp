#include "stdafx.h"
#include "Executable.h"

namespace shared
{

   const std::string CExecutable::Extension() { return "exe"; }
   const std::string CExecutable::DotExtension() { return ".exe"; }

   const std::string CExecutable::ToFileName(const std::string& exeName)
   {
      return exeName + DotExtension();
   }

   const std::string CExecutable::ToExeName(const std::string& fileName)
   {
      boost::filesystem::path exeFile(fileName);
      return exeFile.stem().string();
   }

} // namespace shared