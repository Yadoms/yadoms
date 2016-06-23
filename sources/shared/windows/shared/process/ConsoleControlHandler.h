#pragma once

namespace shared
{
   namespace process
   {
      //-----------------------------
      ///\brief   Class which provide Windows Console handling
      //-----------------------------  
      class CConsoleControlHandler
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
         ///\brief   The Windows console control handler
         ///\param [in]  fdwCtrlType     : information about the control signal
         ///\return  TRUE if the signal has been handled, FALSE other cases
         //-----------------------------  
         static BOOL ctrlHandler(DWORD fdwCtrlType);
      };
   }
} // namespace shared::process


