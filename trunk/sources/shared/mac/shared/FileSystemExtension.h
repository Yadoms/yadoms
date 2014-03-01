//
// FileSystemExtension.h
//
// File system extensions
//
#pragma once

#include <shared/exception/Exception.hpp>
//TODO virer #include <unistd.h>
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
      /// \brief			Macro implementing the getting of the current module path.
      ///               This must be a macro because this code must be executed
      ///               in the module which the path is required.
      //--------------------------------------------------------------
      // #define IMPLEMENT_GET_MODULE_PATH                                                                                 \
      // static const boost::filesystem::path getModulePath()                                                              \
      // {                                                                                                                 \
         // char buff[1024];                                                                                               \
         // ssize_t len = readlink("/proc/self/exe", buff, sizeof(buff)-1);                                                \
         // if (len == -1)                                                                                                 \
         // {                                                                                                              \
            // const std::string error(strerror(errno));                                                                   \
            // throw shared::exception::CException(std::string("CFileSystemExtension::getModulePath fails with error ")    \
               // + error);                                                                                                \
         // }                                                                                                              \
         // return boost::filesystem::path(buff);                                                                          \
      // }
      #define IMPLEMENT_GET_MODULE_PATH                                                                                 \
      static const boost::filesystem::path getModulePath()                                                              \
      {                                                                                                                 \
         Dl_info dl_info;                                                                                               \
         if (dladdr((void *)getModulePath, &dl_info) == 0)                                                              \
         {                                                                                                              \
            const std::string error(strerror(errno));                                                                   \
            throw shared::exception::CException(std::string("CFileSystemExtension::getModulePath fails with error ")    \
               + error);                                                                                                \
         }                                                                                                              \
         return boost::filesystem::path(dl_info.dli_fname);                                                             \
      }

   };

} // namespace shared