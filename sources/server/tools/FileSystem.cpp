#include "stdafx.h"
#include "FileSystem.h"
#include <shared/Log.h>
#include <Poco/File.h>

namespace tools
{
   boost::filesystem::path CFileSystem::createTemporaryFolder()
   {
      const auto tempPath = boost::filesystem::unique_path();
      boost::filesystem::create_directories(tempPath);
      return tempPath;
   }


   bool CFileSystem::exists(const std::string& path)
   {
      Poco::File check(path);
      return check.exists();
   }

   bool CFileSystem::exists(const Poco::Path& path)
   {
      return exists(path.toString());
   }


   void CFileSystem::copyDirectoryContentTo(const Poco::Path& sourceDirectory, const Poco::Path& destination)
   {
      return copyDirectoryContentTo(sourceDirectory.toString(), destination.toString());
   }


   void CFileSystem::copyDirectoryContentTo(const Poco::Path& sourceDirectory, const std::string& destination)
   {
      return copyDirectoryContentTo(sourceDirectory.toString(), destination);
   }


   void CFileSystem::copyDirectoryContentTo(const std::string& sourceDirectory, const std::string& destination)
   {
      Poco::File initialDirectory(sourceDirectory);
      if (initialDirectory.exists() && initialDirectory.isDirectory())
      {
         std::vector<Poco::File> content;
         initialDirectory.list(content);
         for (const auto& i : content)
            i.copyTo(destination);
      }
      else
      {
         throw Poco::FileException("The folder to copy do not exists or is not a directory");
      }
   }

   bool CFileSystem::copyDirectoryRecursivelyTo(boost::filesystem::path const& source, boost::filesystem::path const& destination)
   {
      try
      {
         // Check whether the function call is valid
         if (!boost::filesystem::exists(source) || !boost::filesystem::is_directory(source))
         {
            YADOMS_LOG(error) << "[CFileSystem::copyDir] : Source directory " << source.string() << " does not exist or is not a directory.";
            return false;
         }
         if (boost::filesystem::exists(destination))
         {
            YADOMS_LOG(error) << "[CFileSystem::copyDir] : Destination directory " << destination.string() << " already exists.";
            return false;
         }
         // Create the destination directory
         if (!boost::filesystem::create_directory(destination))
         {
            YADOMS_LOG(error) << "[CFileSystem::copyDir] : Unable to create destination directory" << destination.string();
            return false;
         }
      }
      catch (boost::filesystem::filesystem_error const& e)
      {
         YADOMS_LOG(error) << "[CFileSystem::copyDir] : " << e.what();
         return false;
      }
      // Iterate through the source directory
      for (boost::filesystem::directory_iterator file(source); file != boost::filesystem::directory_iterator(); ++file)
      {
         try
         {
            auto current(file->path());
            if (boost::filesystem::is_directory(current))
            {
               // Found directory: Recursion
               if (!copyDirectoryRecursivelyTo(current, destination / current.filename()))
               {
                  return false;
               }
            }
            else
            {
               // Found file: Copy
               boost::filesystem::copy_file(current, destination / current.filename());
            }
         }
         catch (boost::filesystem::filesystem_error const& e)
         {
            YADOMS_LOG(error) << "[CFileSystem::copyDir] : " << e.what();
         }
      }
      return true;
   }

   void CFileSystem::remove(const Poco::Path& path, bool recursive)
   {
      return remove(path.toString(), recursive);
   }

   void CFileSystem::remove(const std::string& path, bool recursive)
   {
      Poco::File check(path);
      if (check.exists())
      {
         check.remove(recursive);
      }
   }


   void CFileSystem::rename(const Poco::Path& source, const Poco::Path& destination)
   {
      return rename(source.toString(), destination.toString());
   }


   void CFileSystem::rename(const std::string& source, const std::string& destination)
   {
      Poco::File target(source);
      target.renameTo(destination);
   }
} // namespace tools
