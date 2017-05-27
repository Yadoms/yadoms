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


      bool CFileSystem::exists(const std::string & path)
      {
         Poco::File check(path);
         return check.exists();
      }

      bool CFileSystem::exists(const Poco::Path & path)
      {
         return exists(path.toString());
      }



      void CFileSystem::copyDirectoryContentTo(const Poco::Path & sourceDirectory, const Poco::Path & destination)
      {
         return copyDirectoryContentTo(sourceDirectory.toString(), destination.toString());
      }



      void CFileSystem::copyDirectoryContentTo(const Poco::Path & sourceDirectory, const std::string & destination)
      {
         return copyDirectoryContentTo(sourceDirectory.toString(), destination);
      }


      void CFileSystem::copyDirectoryContentTo(const std::string & sourceDirectory, const std::string & destination)
      {
         Poco::File initialDirectory(sourceDirectory);
         if (initialDirectory.exists() && initialDirectory.isDirectory())
         {
            std::vector<Poco::File> content;
            initialDirectory.list(content);
            for (std::vector<Poco::File>::iterator i = content.begin(); i != content.end(); ++i)
            {
               i->copyTo(destination);
            }
         }
         else
         {
            throw Poco::FileException("The folder to copy do not exists or is not a directory");
         }
      }      

      void CFileSystem::remove(const Poco::Path & path, bool recursive)
      {
         return remove(path.toString(), recursive);
      }

      void CFileSystem::remove(const std::string & path, bool recursive)
      {
         Poco::File check(path);
         if (check.exists())
         {
            check.remove(recursive);
         }
      }


      void CFileSystem::rename(const Poco::Path & source, const Poco::Path & destination)
      {
         return rename(source.toString(), destination.toString());
      }


      void CFileSystem::rename(const std::string & source, const std::string & destination)
      {
         Poco::File target(source);
         target.renameTo(destination);
      }

} // namespace tools