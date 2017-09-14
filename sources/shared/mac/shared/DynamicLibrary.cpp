#include "stdafx.h"
#include "DynamicLibrary.h"
#include <dlfcn.h>
#include "../../shared/Log.h"
#include "../../shared/exception/InvalidParameter.hpp"

namespace shared
{
   const std::string CDynamicLibrary::Extension() 
   { 
      return "dylib"; 
   }

   const std::string CDynamicLibrary::DotExtension() 
   { 
      return ".dylib"; 
   }

   const std::string CDynamicLibrary::ToFileName(const std::string& libName)
   {
      return std::string("lib") + libName + DotExtension();
   }

   const std::string CDynamicLibrary::ToLibName(const std::string& fileName)
   {
      boost::filesystem::path libFile(fileName);
      std::string libFileNameWithoutExtension = libFile.stem().string();
      if (boost::starts_with(libFileNameWithoutExtension, "lib"))
         return libFileNameWithoutExtension.substr(3, libFileNameWithoutExtension.length()-3);
      return libFileNameWithoutExtension;
   }

   CDynamicLibrary::CDynamicLibrary()
      :m_libraryHandle(NULL)
   {
   }

   CDynamicLibrary::CDynamicLibrary(const std::string& libraryFile)
      :m_libraryHandle(NULL)
   {
      if (!load(libraryFile))
         throw exception::CInvalidParameter(libraryFile);
   }

   CDynamicLibrary::~CDynamicLibrary()
   {
      if(m_libraryHandle != NULL)
         unload();
   }

   void * CDynamicLibrary::GetFunctionPointer(const std::string & funcName)
   {
      if (m_libraryHandle != NULL) 
      {
         return dlsym(m_libraryHandle, funcName.c_str());	
      }
      return NULL;
   }

   bool CDynamicLibrary::load(const std::string & libraryFile)
   {
      bool result = false;

      //load library
      m_libraryHandle = dlopen(libraryFile.c_str(), RTLD_LAZY);

      if (m_libraryHandle != NULL) 
      {
         result = true;
      } 
      else
      {
         YADOMS_LOG(error) << "Fail to load library : " << libraryFile << dlerror();
      }

      return result;
   }

   void CDynamicLibrary::unload()
   {
      if(m_libraryHandle != NULL)
      {
         dlclose(m_libraryHandle);
         m_libraryHandle = NULL;
      }
   }

} // namespace shared