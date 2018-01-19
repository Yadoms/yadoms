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
      };
   } // namespace tools 
} // namespace shared
