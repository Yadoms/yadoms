#pragma once
#include <shared/Export.h>

namespace shared
{
   namespace tools
   {
      //---------------------------------------------
      ///\brief Filesystem helpers
      //---------------------------------------------
      class YADOMS_SHARED_EXPORT CFilesystem
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
		 ///\brief Compute diretory size
		 ///\param [in] directory Directory to compute size
		 ///\param [in] recurse True to compute directory tree size
		 ///\return the size in bytes
		 //---------------------------------------------
		 static uintmax_t directorySize(const boost::filesystem::path& directory,
										        bool recurse = true);
      };
   } // namespace tools 
} // namespace shared
