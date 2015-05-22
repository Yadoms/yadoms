#include "stdafx.h"
#include "FileSystem.h"
#include <shared/Log.h>
#include <Poco/File.h>

namespace tools
{
      Poco::Path CFileSystem::getTemporaryFolder(const std::string & subFolderName, bool eraseIfExists)
      {
         //get the computer temp folder
         Poco::Path p(Poco::Path::temp());

         //append directory yadoms
         p.pushDirectory("yadoms");
         
         if(!subFolderName.empty())
            p.pushDirectory(subFolderName);

         //remove directory if exists
         Poco::File f(p);
         if (f.exists() && eraseIfExists)
            f.remove(true);

         if(!f.exists())
         {  
            //create directories
            f.createDirectories();
         }
         return p;
      }


} // namespace tools