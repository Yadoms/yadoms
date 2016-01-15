# Macro for getting the WINNT version
#
macro(get_WIN32_WINNT version)
	if (WIN32 AND CMAKE_SYSTEM_VERSION)
		set(ver ${CMAKE_SYSTEM_VERSION})
		string(REPLACE "." "" ver ${ver})
		string(REGEX REPLACE "([0-9])" "0\\1" ver ${ver})

		set(${version} "0x${ver}")
	endif()
endmacro()



macro(FindWindowsDependencies dependencies)
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
            SET(${dependencies} ${dependencies} "${Win10SdkArchDir}/${child}")
         ENDFOREACH()
        
      endif()
      
   ENDIF()
endmacro()