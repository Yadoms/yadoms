#include "stdafx.h"
#include "FileSystem.h"
#include <shared/Log.h>
#include <Poco/File.h>

namespace tools
{
   boost::filesystem::path CFileSystem::createTemporaryFolder()
   {
      const auto tempPath = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path();
      create_directories(tempPath);
      return tempPath;
   }

   bool CFileSystem::exists(const std::string& path)
   {
      return boost::filesystem::exists(path);
   }

   bool CFileSystem::exists(const boost::filesystem::path& path)
   {
      return boost::filesystem::exists(path);
   }

   void CFileSystem::copyDirectoryContentTo(const boost::filesystem::path& sourceDirectory,
                                            const boost::filesystem::path& destination)
   {
      if (!boost::filesystem::exists(sourceDirectory) || !is_directory(sourceDirectory))
         throw std::runtime_error(
            "Copy directory failed : The folder (" + sourceDirectory.string() +
            ") to copy do not exists or is not a directory");

      if (!boost::filesystem::exists(destination))
         if (!create_directory(destination))
            throw std::runtime_error(
               "Copy directory failed : destination directory " + destination.string() + " must exist");

      for (boost::filesystem::directory_iterator file(sourceDirectory);
           file != boost::filesystem::directory_iterator();
           ++file)
      {
         try
         {
            auto current(file->path());
            if (is_directory(current))
            {
               // Found directory: Recursion
               copyDirectoryContentTo(current, destination / current.filename());
            }
            else
            {
               // Found file: Copy
               copy_file(current, destination / current.filename(), boost::filesystem::copy_option::overwrite_if_exists);
            }
         }
         catch (boost::filesystem::filesystem_error const& e)
         {
            throw std::runtime_error(std::string("Copy directory failed : ") + e.what());
         }
      }
   }

   bool CFileSystem::copyDirectoryRecursivelyTo(const boost::filesystem::path& source,
                                                const boost::filesystem::path& destination)
   {
      try
      {
         // Check whether the function call is valid
         if (!boost::filesystem::exists(source) || !is_directory(source))
         {
            YADOMS_LOG(error) << "[CFileSystem::copyDir] : Source directory " << source.string() <<
               " does not exist or is not a directory.";
            return false;
         }
         if (boost::filesystem::exists(destination))
         {
            YADOMS_LOG(error) << "[CFileSystem::copyDir] : Destination directory " << destination.string() <<
               " already exists.";
            return false;
         }
         // Create the destination directory
         if (!create_directory(destination))
         {
            YADOMS_LOG(error) << "[CFileSystem::copyDir] : Unable to create destination directory" << destination.
               string();
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
            if (is_directory(current))
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
               copy_file(current, destination / current.filename(), boost::filesystem::copy_option::overwrite_if_exists);
            }
         }
         catch (boost::filesystem::filesystem_error const& e)
         {
            YADOMS_LOG(error) << "[CFileSystem::copyDir] : " << e.what();
         }
      }
      return true;
   }

   void CFileSystem::remove(const boost::filesystem::path& path, bool recursive)
   {
      return remove(path.string(), recursive);
   }

   void CFileSystem::remove(const std::string& path, bool recursive)
   {
      Poco::File check(path);
      if (check.exists())
      {
         check.remove(recursive);
      }
   }

   void CFileSystem::rename(const boost::filesystem::path& source,
                            const boost::filesystem::path& destination)
   {
      return rename(source.string(), destination.string());
   }

   void CFileSystem::rename(const std::string& source,
                            const std::string& destination)
   {
      Poco::File target(source);
      target.renameTo(destination);
   }
} // namespace tools
