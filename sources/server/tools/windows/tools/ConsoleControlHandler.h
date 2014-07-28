#pragma once
#include "tools/IApplication.h"

namespace tools {

   //-----------------------------
   ///\brief   Class (static) which provide Windows Console handling
   //-----------------------------  
   class CConsoleControlHandler
   {
   public:
      //-----------------------------
      ///\brief   Register the application to manage in case of console exit
      ///\param [in]  app     : The application to manage
      //-----------------------------  
      static void registerApplication(tools::IApplication & app);

   private:
      //-----------------------------
      ///\brief   The Windows console control handler
      ///\param [in]  fdwCtrlType     : information about the control signal
      ///\return  TRUE if the signal has been handled, FALSE other cases
      //-----------------------------  
      static BOOL ctrlHandler(DWORD fdwCtrlType);

      //-----------------------------
      ///\brief   Callback used when application is really stopped
      //-----------------------------  
      static void onAppStopped();

   private:
      //-----------------------------
      ///\brief   Pointer to application to manage
      //-----------------------------  
      static tools::IApplication * m_pApplication;

      //-----------------------------
      ///\brief   Event for waiting application to close
      //-----------------------------  
      static HANDLE m_hStoppedEvent;
   };
   

} //namespace tools