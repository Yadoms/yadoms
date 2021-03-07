#include "stdafx.h"
#include "Filesystem.h"

#include "shared/Log.h"


namespace shared
{
   namespace tools
   {
      boost::filesystem::path CFilesystem::makeRelative(const boost::filesystem::path& from,
                                                        const boost::filesystem::path& to)
      {
         // Start at the root path and while they are the same then do nothing then when they first
         // diverge take the entire from path, swap it with '..' segments, and then append the remainder of the to path.
         auto fromIter = from.begin();
         auto toIter = to.begin();

         // Loop through both while they are the same to find nearest common directory
         while (fromIter != from.end() && toIter != to.end() && (*toIter) == (*fromIter))
         {
            ++toIter;
            ++fromIter;
         }

         // Replace from path segments with '..' (from => nearest common directory)
         boost::filesystem::path finalPath;
         while (fromIter != from.end())
         {
            finalPath /= "..";
            ++fromIter;
         }

         // Append the remainder of the to path (nearest common directory => to)
         while (toIter != to.end())
         {
            finalPath /= *toIter;
            ++toIter;
         }

         return finalPath;
      }

      uintmax_t CFilesystem::directorySize(const boost::filesystem::path& directory,
                                           bool recurse)
      {
         uintmax_t size = 0;

         const auto& path = directory;
         for (auto itDir = boost::filesystem::directory_iterator(path); itDir != boost::filesystem::directory_iterator(); ++itDir)
         {
            if (is_directory(*itDir))
            {
               if (recurse)
                  size += directorySize(*itDir, true);

               continue;
            }
            size += file_size(*itDir);
         }
         return size;
      }


      boost::filesystem::path CFilesystem::createTemporaryFolder()
      {
         const auto tempPath = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path();
         create_directories(tempPath);
         return tempPath;
      }

      bool CFilesystem::exists(const std::string& path)
      {
         return boost::filesystem::exists(path);
      }

      bool CFilesystem::exists(const boost::filesystem::path& path)
      {
         return boost::filesystem::exists(path);
      }

      void CFilesystem::copyDirectoryContentTo(const boost::filesystem::path& sourceDirectory,
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

      bool CFilesystem::copyDirectoryRecursivelyTo(const boost::filesystem::path& source,
                                                   const boost::filesystem::path& destination)
      {
         try
         {
            // Check whether the function call is valid
            if (!boost::filesystem::exists(source) || !is_directory(source))
            {
               YADOMS_LOG(error) << "[CFilesystem::copyDir] : Source directory " << source.string() <<
                  " does not exist or is not a directory.";
               return false;
            }
            if (boost::filesystem::exists(destination))
            {
               YADOMS_LOG(error) << "[CFilesystem::copyDir] : Destination directory " << destination.string() <<
                  " already exists.";
               return false;
            }
            // Create the destination directory
            if (!create_directory(destination))
            {
               YADOMS_LOG(error) << "[CFilesystem::copyDir] : Unable to create destination directory" << destination.
                  string();
               return false;
            }
         }
         catch (boost::filesystem::filesystem_error const& e)
         {
            YADOMS_LOG(error) << "[CFilesystem::copyDir] : " << e.what();
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
               YADOMS_LOG(error) << "[CFilesystem::copyDir] : " << e.what();
            }
         }
         return true;
      }

      void CFilesystem::remove(const boost::filesystem::path& path, bool recursive)
      {
         if (boost::filesystem::exists(path))
            return remove(path.string(), recursive);
      }

      void CFilesystem::rename(const boost::filesystem::path& source,
                               const boost::filesystem::path& destination)
      {
         return boost::filesystem::rename(source, destination);
      }
   } // namespace tools 
} // namespace shared
