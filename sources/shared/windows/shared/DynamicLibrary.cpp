#include "stdafx.h"

#include "DynamicLibrary.h"
#include "../../shared/Log.h"
#include "../../shared/exception/InvalidParameter.hpp"
#include <Windows.h>

#ifndef LoadLibrary
   #ifdef UNICODE
   #define LoadLibrary  LoadLibraryW
   #else
   #define LoadLibrary  LoadLibraryA
   #endif // !UNICODE
#endif

namespace shared
{

   const std::string CDynamicLibrary::Extension() { return "dll"; }
   const std::string CDynamicLibrary::DotExtension() { return ".dll"; }

   const std::string CDynamicLibrary::ToFileName(const std::string& libName)
   {
      return libName + DotExtension();
   }

   const std::string CDynamicLibrary::ToLibName(const std::string& fileName)
   {
      boost::filesystem::path libFile(fileName);
      return libFile.stem().string();
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

   void* CDynamicLibrary::GetFunctionPointer(const std::string& funcName)
   {
      BOOST_ASSERT(m_libraryHandle);
      if (m_libraryHandle == NULL)
      {
         YADOMS_LOG(error) << "Library not loaded";
         return NULL;
      }

      return GetProcAddress((HMODULE)m_libraryHandle, funcName.c_str());
   }

   bool CDynamicLibrary::load(const std::string& libraryFile)
   {
      unload();
      m_libraryHandle = LoadLibrary(libraryFile.c_str());

      if (m_libraryHandle == NULL)
      {
         YADOMS_LOG(error) << "Fail to load library : " << libraryFile << ", erreur : " << GetLastError();
         return false;
      }

      return true;
   }

   void CDynamicLibrary::unload()
   {
      if(m_libraryHandle != NULL)
      {
         FreeLibrary((HMODULE)m_libraryHandle);
         m_libraryHandle = NULL;
      }
   }



} // namespace shared