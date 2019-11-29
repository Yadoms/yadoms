#pragma once

namespace shared
{

   //--------------------------------------------------------------
   /// \brief	This class is used to get information on executable files
   //--------------------------------------------------------------
   class CExecutable
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Returns platform standard executable extension
      //--------------------------------------------------------------
      static std::string Extension();
      static std::string DotExtension();

      //--------------------------------------------------------------
      /// \brief	Conversion fileName (platform-dependent) <=> exeName (non-platform-dependent)
      //--------------------------------------------------------------
      static std::string ToFileName(const std::string& exeName);
      static std::string ToExeName(const std::string& fileName);
   };

} // namespace shared