#pragma once

namespace shared
{
   //--------------------------------------------------------------
   /// \brief	Windows File system extensions
   //--------------------------------------------------------------
   class CFileSystemExtension final
   {
   public:
      ~CFileSystemExtension() = default;

      //--------------------------------------------------------------
      /// \brief			Get the current module full path (drive + path + filename + ext).
      ///               This code must be executed in the module which the path is required,
      ///               so let it in the header file.
      //--------------------------------------------------------------
      static boost::filesystem::path getModuleFullPath();

      //--------------------------------------------------------------
      /// \brief			Get the current module path  (drive + path, without filename and ext).
      ///               This code must be executed in the module which the path is required,
      ///               so let it in the header file.
      //--------------------------------------------------------------
      static boost::filesystem::path getModulePath();
   };
}
