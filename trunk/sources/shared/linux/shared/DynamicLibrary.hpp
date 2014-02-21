#pragma once
#include <string>
#include <dlfcn.h>
#include <boost/function.hpp>
#include <boost/log/trivial.hpp>
#include "../../shared/Log.h"

namespace shared
{

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
      static const std::string Extension() { return "so"; }
      static const std::string DotExtension() { return ".so"; }

      //--------------------------------------------------------------
      /// \brief	Conversion fileName (platform-dependent) <=> libName (non-platform-dependent)
      //--------------------------------------------------------------
      static const std::string ToFileName(const std::string& libName)
      {
	      return std::string("lib") + libName + DotExtension();
      }

      static const std::string ToLibName(const std::string& libName)
      {
	      boost::filesystem::path libFile(libName);
	      std::string libFileNameWithoutExtension = libFile.stem().string();
	      if (boost::starts_with(libFileNameWithoutExtension, "lib"))
		      return libFileNameWithoutExtension.substr(3, libFileNameWithoutExtension.length()-3);
	      return libFileNameWithoutExtension;
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
           BOOST_ASSERT(m_libraryHandle == NULL);
       }

       //--------------------------------------------------------------
       /// \brief	    Get a function pointer
       /// \param [in] funcName: the exported function to search
       /// \return     a function pointer or NULL if file is not loaded or funtion is not found
       //-------------------------------------------------------------
       void * GetFunctionPointer(const std::string & funcName)
       {
           if (m_libraryHandle != NULL) 
           {
               return dlsym(m_libraryHandle, funcName.c_str());	
           }
           return NULL;
       }



       //--------------------------------------------------------------
       /// \brief	    Loads a library file
       /// \param [in] libraryFile: the library file path
       /// \return     true if loaded with success, else false
       //-------------------------------------------------------------
       virtual bool load(const std::string & libraryFile)
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

       //--------------------------------------------------------------
       /// \brief	    Free library file
       //-------------------------------------------------------------
       void unload()
       {
           if(m_libraryHandle != NULL)
           {
               dlclose(m_libraryHandle);
               m_libraryHandle = NULL;
           }
       }

   private:
       //-------------------------------------------------------------
       /// \brief	    The library handle
       //-------------------------------------------------------------
       void * m_libraryHandle;
   };

} // namespace shared