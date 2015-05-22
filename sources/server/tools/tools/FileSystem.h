#pragma once
#include <shared/StringExtension.h>
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
      static Poco::Path getTemporaryFolder(const std::string & subFolderName = shared::CStringExtension::EmptyString, bool eraseIfExists = false);

   };

} // namespace tools