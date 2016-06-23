#pragma once

namespace shared
{
   namespace process
   {
      //-----------------------------
      ///\brief   Class which provide Windows Service handling
      //-----------------------------  
      class CServiceControlHandler
      {
      public:
         //-----------------------------
         ///\brief   Set an handler on stop requested
         ///\param[in] onStopRequestedFct Handler to call when stop is requested
         ///\details The handler to call when stop is requested takes another handler, to call by app when stop is finished
         //----------------------------- 
         static void setOnStopRequestedHandler(boost::function<bool()> onStopRequestedFct);

      private:
         static boost::function<bool()> m_onStopRequestedFct;

         //-----------------------------
         ///\brief   The service console control handler
         ///\param [in]  control    : information about the control signal
         //-----------------------------  
         static void ServiceControlHandler(DWORD control);

         static SERVICE_STATUS m_serviceStatus;
         static SERVICE_STATUS_HANDLE m_serviceStatusHandle;
      };
   }
} // namespace shared::process


