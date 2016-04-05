#pragma once

#include <shared/Export.h>

namespace shared
{

   //--------------------------------------------------------------
   /// \brief	This class is used to get information on executable files
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CExecutable
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Returns platform standard executable extension
      //--------------------------------------------------------------
      static const std::string Extension();
      static const std::string DotExtension();

      //--------------------------------------------------------------
      /// \brief	Conversion fileName (platform-dependent) <=> exeName (non-platform-dependent)
      //--------------------------------------------------------------
      static const std::string ToFileName(const std::string& exeName);
      static const std::string ToExeName(const std::string& fileName);
   };

} // namespace shared