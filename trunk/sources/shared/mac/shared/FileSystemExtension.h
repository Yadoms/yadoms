//
// FileSystemExtension.h
//
// File system extensions
//
#pragma once

#include <shared/exception/Exception.hpp>
#include <dlfcn.h>

namespace shared
{

   //--------------------------------------------------------------
   /// \brief	this class is used to manage platforms peripherals
   //--------------------------------------------------------------
   class CFileSystemExtension
   {
   public:
      //--------------------------------------------------------------
      /// \brief			Constructor
      //--------------------------------------------------------------
      CFileSystemExtension();

      //--------------------------------------------------------------
      /// \brief			Destructor
      //--------------------------------------------------------------
      virtual ~CFileSystemExtension();

      //--------------------------------------------------------------
      /// \brief			Get the current module full path (drive + path + filename + ext).
      ///               This code must be executed in the module which the path is required,
      ///               so let it in the header file.
      //--------------------------------------------------------------
      static inline boost::filesystem::path getModuleFullPath(void * ptr)
      {
          void * ptrToUse = ptr;
          if(ptrToUse == NULL)
              ptrToUse = (void*)getModuleFullPath;
          
         Dl_info dl_info; 
         if (dladdr(ptrToUse, &dl_info) == 0)
         { 
            const std::string error(strerror(errno));
            throw shared::exception::CException(std::string("CFileSystemExtension::getModuleFullPath fails with error ") + error);
         } 
         
         /* We've got the path, construct a path from it */
         return boost::filesystem::path(dl_info.dli_fname);
      }

      //--------------------------------------------------------------
      /// \brief			Get the current module path.
      ///               This code must be executed in the module which the path is required,
      ///               so let it in the header file.
      //--------------------------------------------------------------
      static inline boost::filesystem::path getModulePath(void * ptr = NULL)
      {
         return getModuleFullPath(ptr).parent_path();
      }

   };

} // namespace shared