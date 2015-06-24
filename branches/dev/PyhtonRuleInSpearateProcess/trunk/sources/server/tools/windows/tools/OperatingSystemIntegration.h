#pragma once
#include "tools/IOperatingSystemIntegration.h"

namespace tools {

   //-----------------------------
   ///\brief Class which integrate Yadoms in OperatingSystem
   /// It allow Console control, systray icon,...
   //-----------------------------   
   class COperatingSystemIntegration : public IOperatingSystemIntegration
   {
   public:
      //-----------------------------
      ///\brief Constructor
      //-----------------------------   
      COperatingSystemIntegration();
      
      //-----------------------------
      ///\brief Destructor
      //-----------------------------         
      virtual ~COperatingSystemIntegration();
      
      // IOperatingSystemIntegration implementation
      virtual void configure();
      // [END] - IOperatingSystemIntegration implementation
   };

} //namespace tools