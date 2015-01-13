//
// FileSystemExtension.h
//
// File system extensions
//
#pragma once

#include <shared/exception/Exception.hpp>

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
      static boost::filesystem::path getModuleFullPath()
      {
         std::vector<char> modulePath(MAX_PATH);

         HMODULE hModule = NULL;
         GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)getModulePath, &hModule);
         if (hModule == NULL)
         {
            DWORD error = GetLastError();                                                                               
            throw shared::exception::CException(std::string("CFileSystemExtension::getModuleFullPath fails with error ")    
               + boost::lexical_cast<std::string>(error));                                                              
         }                                                                                                              

         /* Try to get the executable path with a buffer of MAX_PATH characters. */                                     
         DWORD result = GetModuleFileNameA(hModule, &modulePath[0], static_cast<DWORD>(modulePath.size()));             

         /* As long the function returns the buffer size, it is indicating that the buffer */                           
         /* was too small. Keep enlarging the buffer by a factor of 2 until it fits. */                                 
         while(result == modulePath.size())                                                                             
         {                                                                                                              
            modulePath.resize(modulePath.size() * 2);                                                                   
            result = GetModuleFileNameA(hModule, &modulePath[0], static_cast<DWORD>(modulePath.size()));                
         }                                                                                                              

         /* If the function returned 0, something went wrong */                                                         
         if(result == 0)                                                                                                
         {                                                                                                              
            DWORD error = GetLastError();                                                                               
            throw shared::exception::CException(std::string("CFileSystemExtension::getModuleFullPath fails with error ")    
               + boost::lexical_cast<std::string>(error));                                                              
         }                                                                                                              

         /* We've got the path, construct a path from it */                                                             
         return boost::filesystem::path(std::string(modulePath.begin(), modulePath.begin() + result));                
      }

      //--------------------------------------------------------------
      /// \brief			Get the current module path.
      ///               This code must be executed in the module which the path is required,
      ///               so let it in the header file.
      //--------------------------------------------------------------
      static boost::filesystem::path getModulePath()
      {
         return getModuleFullPath().parent_path();                                                                                 
      }
   };

} // namespace shared