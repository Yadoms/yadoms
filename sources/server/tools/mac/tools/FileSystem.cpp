#include "stdafx.h"
#include "../../FileSystem.h"

namespace tools
{
   Poco::Path CFileSystem::getLogFolder()
   {
      static const Poco::Path logPath("/var/log");
      return logPath;
   }
} // namespace tools


