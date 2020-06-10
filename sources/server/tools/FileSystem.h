#pragma once

namespace tools
{
   //---------------------------------------------
   ///\brief Class which provide file system methods
   //---------------------------------------------
   class CFileSystem
   {
   public:

      //---------------------------------------------
      ///\brief Get a temporary folder
      ///\return The temporary folder path
      //---------------------------------------------   
      static boost::filesystem::path createTemporaryFolder();

      //---------------------------------------------
      ///\brief Check if a file/directory exists
      ///\param [in] path     The file/directory path to check
      ///\return true if exists, else false
      //---------------------------------------------   
      static bool exists(const std::string& path);

      //---------------------------------------------
      ///\brief Check if a file/directory exists
      ///\param [in] path     The file/directory path to check
      ///\return true if exists, else false
      //---------------------------------------------   
      static bool exists(const boost::filesystem::path& path);

      //---------------------------------------------
      ///\brief Copy the directory content to a destination
      ///\param [in] sourceDirectory   The source directory (the directory itself is not copied; only its content)
      ///\param [in] destination       The destination directory 
      //---------------------------------------------   
      static void copyDirectoryContentTo(const boost::filesystem::path& sourceDirectory,
                                         const boost::filesystem::path& destination);

      //---------------------------------------------
      ///\brief Copy the directory content to a destination recursively, using boost::filesystem functions
      ///\param [in] source            The source directory (the directory itself is not copied; only its content) => must exist
      ///\param [in] destination       The destination directory => must NOT exists
      ///\return true for success, false for other cases
      // example
      //
      //  folderA / file1
      //          / file2
      //          / folder1
      //                / file 3
      //                / file 4
      //
      // copyDir(folderA, anyDestination)
      //  anyDestination   / file1
      //                   / file2
      //                   / folder1
      //                      / file 3
      //                      / file 4
      //
      //---------------------------------------------   
      static bool copyDirectoryRecursivelyTo(const boost::filesystem::path& source,
                                             const boost::filesystem::path& destination);

      //---------------------------------------------
      ///\brief Remove a file/directory 
      ///\param [in] path     The file/directory path to delete
      ///\param [in] recursive     if true remove directory content (only available for directories; ignored for files)
      //---------------------------------------------   
      static void remove(const boost::filesystem::path& path,
                         bool recursive = true);

      //---------------------------------------------
      ///\brief Remove a file/directory 
      ///\param [in] path     The file/directory path to delete
      ///\param [in] recursive     if true remove directory content (only available for directories; ignored for files)
      //---------------------------------------------   
      static void remove(const std::string& path,
                         bool recursive = true);

      //---------------------------------------------
      ///\brief Rename a file/directory
      ///\param [in] source            The source file/directory
      ///\param [in] destination       The destination directory 
      //---------------------------------------------   
      static void rename(const std::string& source,
                         const std::string& destination);

      //---------------------------------------------
      ///\brief Rename a file/directory
      ///\param [in] source            The source file/directory
      ///\param [in] destination       The destination directory 
      //---------------------------------------------   
      static void rename(const boost::filesystem::path& source,
                         const boost::filesystem::path& destination);
   };
} // namespace tools
