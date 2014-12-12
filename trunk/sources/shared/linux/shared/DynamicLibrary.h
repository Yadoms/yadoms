#pragma once
#include <shared/Export.h>

namespace shared
{

   //--------------------------------------------------------------
   /// \brief	This class is used to load a plugin file library and 
   ///         provide construct and destruct methods
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CDynamicLibrary
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Returns platform standard dynamic library extension
      //--------------------------------------------------------------
      static const std::string Extension();
      static const std::string DotExtension();

      //--------------------------------------------------------------
      /// \brief	Conversion fileName (platform-dependent) <=> libName (non-platform-dependent)
      //--------------------------------------------------------------
      static const std::string ToFileName(const std::string& libName);
      static const std::string ToLibName(const std::string& fileName);

   protected:
       //--------------------------------------------------------------
       /// \brief	Constructor
       //--------------------------------------------------------------
       CDynamicLibrary();

       //--------------------------------------------------------------
       /// \brief	Destructor
       //--------------------------------------------------------------
       virtual ~CDynamicLibrary();

       //--------------------------------------------------------------
       /// \brief	    Get a function pointer
       /// \param [in] funcName: the exported function to search
       /// \return     a function pointer or NULL if file is not loaded or funtion is not found
       //-------------------------------------------------------------
       void * GetFunctionPointer(const std::string & funcName);

       //--------------------------------------------------------------
       /// \brief	    Loads a library file
       /// \param [in] libraryFile: the library file path
       /// \return     true if loaded with success, else false
       //-------------------------------------------------------------
       virtual bool load(const std::string & libraryFile);

       //--------------------------------------------------------------
       /// \brief	    Free library file
       //-------------------------------------------------------------
       void unload();
       
   private:
       //-------------------------------------------------------------
       /// \brief	    The library handle
       //-------------------------------------------------------------
       void * m_libraryHandle;
   };

} // namespace shared