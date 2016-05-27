#pragma once
#include <shared/process/IOperatingSystemIntegration.h>

namespace shared
{
   namespace process
   {
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
         void setApplicationStopRequestHandler(boost::function<void(boost::function<void()>)> onStopRequestedFct) override;
         // [END] - IOperatingSystemIntegration implementation
      };
   }
} // namespace shared::process


