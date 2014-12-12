#include "stdafx.h"
#include "DynamicLibrary.h"
#include <boost/function.hpp>
#include <boost/log/trivial.hpp>
#include "../../shared/Log.h"
#include <Windows.h>

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
   {
      m_libraryHandle = NULL;
   }

   CDynamicLibrary::~CDynamicLibrary()
   {
      BOOST_ASSERT(m_libraryHandle == NULL);  // Library was not unload
   }

   void* CDynamicLibrary::GetFunctionPointer(const std::string& funcName)
   {
      BOOST_ASSERT(m_libraryHandle);
      if (m_libraryHandle == NULL)
      {
         YADOMS_LOG(error) << "Library not loaded";
         return NULL;
      }

      return GetProcAddress(m_libraryHandle, funcName.c_str());	
   }

   bool CDynamicLibrary::load(const std::string& libraryFile)
   {
      m_libraryHandle = LoadLibrary(libraryFile.c_str());

      if (m_libraryHandle == NULL)
      {
         DWORD lastError = GetLastError();
         YADOMS_LOG(error) << "Fail to load library : " << libraryFile << ", erreur : " << lastError;
         return false;
      }

      return true;
   }

   void CDynamicLibrary::unload()
   {
      if(m_libraryHandle != NULL)
      {
         FreeLibrary(m_libraryHandle);
         m_libraryHandle = NULL;
      }
   }



} // namespace shared