#pragma once
#include "tools/IApplication.h"
#include "tools/service/IApplicationServiceInformation.h"

namespace tools {

   //-----------------------------
   ///\brief   Class used to start an application
   ///         The aim is to add service behavior, so the software could be launched as a windows service or as console app
   //-----------------------------   
   class CApplicationLoader
   {
   public:
      //-----------------------------
      ///\brief   Start an application
      ///\param [in ] application      The application to start
      ///\param [in ] application      The application service information
      ///\param [in ] argc             The number of arguments
      ///\param [in ] argv             The arguments
      ///\return The application return code
      //----------------------------- 
      static int run(IApplication & application, service::IApplicationServiceInformation & info, int argc, char ** argv);
      
   };

} //namespace tools