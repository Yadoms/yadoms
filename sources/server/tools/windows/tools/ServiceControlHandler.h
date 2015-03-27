#pragma once
#include "tools/IApplication.h"
#include <shared/event/EventHandler.hpp>

namespace tools {

   //-----------------------------
   ///\brief   Class (static) which provide Windows Service handling
   //-----------------------------  
   class CServiceControlHandler
   {
   public:
      static void ConfigureServiceControl();
   private:
      //-----------------------------
      ///\brief   The service console control handler
      ///\param [in]  control    : information about the control signal
      //-----------------------------  
      static void ServiceControlHandler(DWORD control);
     
      static SERVICE_STATUS        _serviceStatus; 
      static SERVICE_STATUS_HANDLE _serviceStatusHandle; 
   
   private:
      //-----------------------------
      ///\brief   Event handler
      //-----------------------------  
      static boost::shared_ptr<shared::event::CEventHandler> m_eventHandler;

      //-----------------------------
      ///\brief   Event for waiting application to close
      //-----------------------------
      enum
      {
         kAppEnd = shared::event::kUserFirstId
      };      
   };
   

} //namespace tools