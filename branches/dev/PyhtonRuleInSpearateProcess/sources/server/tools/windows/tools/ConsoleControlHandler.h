#pragma once
#include "tools/IApplication.h"
#include <shared/event/EventHandler.hpp>

namespace tools {

   //-----------------------------
   ///\brief   Class (static) which provide Windows Console handling
   //-----------------------------  
   class CConsoleControlHandler
   {
   public:
      static void ConfigureConsoleControl();
   private:
      //-----------------------------
      ///\brief   The Windows console control handler
      ///\param [in]  fdwCtrlType     : information about the control signal
      ///\return  TRUE if the signal has been handled, FALSE other cases
      //-----------------------------  
      static BOOL ctrlHandler(DWORD fdwCtrlType);

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