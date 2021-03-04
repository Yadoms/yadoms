#pragma once

namespace shared
{
   namespace tools
   {
      //---------------------------------------------
      ///\brief Filesystem helpers
      //---------------------------------------------
      class CFilesystem //TODO faire en sorte que ces fonctions soient utilisées partout dans le code plutôt que poco ou boost
      {
      public:
         //---------------------------------------------
         ///\brief Make a path relative to another
         ///\param [in] from  The initial path
         ///\param [in] to  The reference path
         ///\return a new path pointing to 'from' and relative to 'to'
         //---------------------------------------------
         static boost::filesystem::path makeRelative(const boost::filesystem::path& from,
                                                     const boost::filesystem::path& to);

         //---------------------------------------------
         ///\brief Compute directory size
         ///\param [in] directory Directory to compute size
         ///\param [in] recurse True to compute directory tree size
         ///\return the size in bytes
         //---------------------------------------------
         static uintmax_t directorySize(const boost::filesystem::path& directory,
                                        bool recurse = true);

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
         ///\brief Copy the directory content to a destination recursively,
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
         ///\brief Rename a file/directory
         ///\param [in] source            The source file/directory
         ///\param [in] destination       The destination directory 
         //---------------------------------------------   
         static void rename(const boost::filesystem::path& source,
                            const boost::filesystem::path& destination);
      };
   } // namespace tools 
} // namespace shared
