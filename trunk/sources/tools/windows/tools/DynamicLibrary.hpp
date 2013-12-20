//
// PluginFactory.hpp
//
// yadoms-plugin factory (base)
//
#pragma once

#include <string>
#include <boost/function.hpp>
#include <boost/log/trivial.hpp>
#include "../../tools/Log.h"
#include "Windows.h"

//--------------------------------------------------------------
/// \brief	This class is used to load a plugin file library and 
///         provide construct and destruct methods
//--------------------------------------------------------------
class CDynamicLibrary
{
public:
   //--------------------------------------------------------------
   /// \brief	Returns platform standard dynamic library extension
   //--------------------------------------------------------------
   static const std::string Extension() { return "dll"; }
   static const std::string DotExtension() { return ".dll"; }

   //--------------------------------------------------------------
   /// \brief	Conversion fileName (platform-dependent) <=> libName (non-platform-dependent)
   //--------------------------------------------------------------
   static const std::string ToFileName(const std::string& libName)
   {
	   return libName + DotExtension();
   }

   static const std::string ToLibName(const std::string& libName)
   {
	   boost::filesystem::path libFile(libName);
	   return libFile.stem().string();
   }

protected:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CDynamicLibrary()
   {
      m_libraryHandle = NULL;
   }

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CDynamicLibrary()
   {
      BOOST_ASSERT(m_libraryHandle == NULL);  // Library was not unload
   }


   //--------------------------------------------------------------
   /// \brief	    Get a function pointer
   /// \param [in] funcName: the exported function to search
   /// \return     a function pointer or NULL if file is not loaded or funtion is not found
   //-------------------------------------------------------------
   void* GetFunctionPointer(const std::string& funcName)
   {
      BOOST_ASSERT(m_libraryHandle);
      if (m_libraryHandle == NULL)
      {
         YADOMS_LOG(error) << "Library not loaded";
         return NULL;
      }

      return GetProcAddress(m_libraryHandle, funcName.c_str());	
   }


   //--------------------------------------------------------------
   /// \brief	    Loads a library file
   /// \param [in] libraryFile: the library file path
   /// \return     true if loaded with success, else false
   //-------------------------------------------------------------
   virtual bool load(const std::string& libraryFile)
   {
      m_libraryHandle = LoadLibrary(libraryFile.c_str());

      if (m_libraryHandle == NULL)
      {
         YADOMS_LOG(error) << "Fail to load library : " << libraryFile << GetLastError();
         return false;
      }

      return true;
   }

   //--------------------------------------------------------------
   /// \brief	    Free library file
   //-------------------------------------------------------------
   void unload()
   {
      if(m_libraryHandle != NULL)
      {
         FreeLibrary(m_libraryHandle);
         m_libraryHandle = NULL;
      }
   }

private:
   //-------------------------------------------------------------
   /// \brief	    The library handle
   //-------------------------------------------------------------
   HMODULE m_libraryHandle;
};

