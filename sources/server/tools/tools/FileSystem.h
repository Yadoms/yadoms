#pragma once
#include <Poco/Path.h>

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
      ///\param [in] subFolderName  The subfolder name
      ///\param [in] eraseIfExists  If true the directory content is removed
      ///\return The temporary folder path
      //---------------------------------------------   
      static Poco::Path getTemporaryFolder(const std::string & subFolderName = std::string(), bool eraseIfExists = false);

      //---------------------------------------------
      ///\brief Check if a file/directory exists
      ///\param [in] path     The file/directory path to check
      ///\return true if exists, else false
      //---------------------------------------------   
      static bool exists(const std::string & path);
      
      //---------------------------------------------
      ///\brief Check if a file/directory exists
      ///\param [in] path     The file/directory path to check
      ///\return true if exists, else false
      //---------------------------------------------   
      static bool exists(const Poco::Path & path);
      
      //---------------------------------------------
      ///\brief Copy the directory content to a destination
      ///\param [in] sourceDirectory   The source directory (the directory itself is not copied; only its content)
      ///\param [in] destination       The destination directory 
      //---------------------------------------------   
      static void copyDirectoryContentTo(const Poco::Path & sourceDirectory, const Poco::Path & destination);    

      //---------------------------------------------
      ///\brief Copy the directory content to a destination
      ///\param [in] sourceDirectory   The source directory (the directory itself is not copied; only its content)
      ///\param [in] destination       The destination directory 
      //---------------------------------------------   
      static void copyDirectoryContentTo(const Poco::Path & sourceDirectory, const std::string & destination);
      
      //---------------------------------------------
      ///\brief Copy the directory content to a destination
      ///\param [in] sourceDirectory   The source directory (the directory itself is not copied; only its content)
      ///\param [in] destination       The destination directory 
      //---------------------------------------------   
      static void copyDirectoryContentTo(const std::string & sourceDirectory, const std::string & destination);

      //---------------------------------------------
      ///\brief Remove a file/directory 
      ///\param [in] path     The file/directory path to delete
      ///\param [in] path     if true remove direcotry content (only available for directories; ignored for files)
      //---------------------------------------------   
      static void remove(const Poco::Path & path, bool recursive);

      //---------------------------------------------
      ///\brief Remove a file/directory 
      ///\param [in] path     The file/directory path to delete
      ///\param [in] path     if true remove direcotry content (only available for directories; ignored for files)
      //---------------------------------------------   
      static void remove(const std::string & path, bool recursive);

      //---------------------------------------------
      ///\brief Rename a file/directory
      ///\param [in] sourceDirectory   The source file/directory
      ///\param [in] destination       The destination directory 
      //---------------------------------------------   
      static void rename(const std::string & source, const std::string & destination); 
      
      //---------------------------------------------
      ///\brief Rename a file/directory
      ///\param [in] sourceDirectory   The source file/directory
      ///\param [in] destination       The destination directory 
      //---------------------------------------------   
      static void rename(const Poco::Path & source, const Poco::Path & destination);

   };

} // namespace tools