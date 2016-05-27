#pragma once

namespace shared
{
   namespace process
   {
      //-----------------------------
      ///\brief Interface for operating system integration
      //-----------------------------
      class IOperatingSystemIntegration
      {
      public:
         //-----------------------------
         ///\brief Virtual destructor
         //-----------------------------   
         virtual ~IOperatingSystemIntegration()
         {
         }

         //-----------------------------
         ///\brief   Set an handler on stop requested by OS
         ///\param[in] onStopRequestedFct Handler to call when stop is requested
         ///\details The handler to call when stop is requested takes another handler, to call by app when stop is finished
         //----------------------------- 
         virtual void setApplicationStopRequestHandler(boost::function<void(boost::function<void()>)> onStopRequestedFct) = 0;
      };
   }
} // namespace shared::process


