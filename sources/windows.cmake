#This macro add some SDK dependencies to INSTALL
macro(InstallWindowsSdkDependencies)
   #in case of Windows 10, we should append universal crt files
   #could be found in Windows 10 sdk
   
   get_filename_component(_win10sdkdir
                           "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot10]"
                           ABSOLUTE)

   IF(EXISTS "${_win10sdkdir}")
   
      #choose x86/x64
      if(CMAKE_SIZEOF_VOID_P MATCHES "8")
         SET(Win10SdkArchDir ${_win10sdkdir}/Redist/ucrt/DLLs/x64)
      else()
         SET(Win10SdkArchDir ${_win10sdkdir}/Redist/ucrt/DLLs/x86)
      endif()
   
      #search for files
      IF(EXISTS "${Win10SdkArchDir}")
      
         message(STATUS "Windows 10 SDK found, add it as dependencies")
         
         FILE(GLOB children RELATIVE "${Win10SdkArchDir}" "${Win10SdkArchDir}/*.dll")
         FOREACH(child ${children})
            INSTALL(FILES "${Win10SdkArchDir}/${child}" DESTINATION ${INSTALL_BINDIR})
         ENDFOREACH()
      endif()
      
   ENDIF()
endmacro()