#pragma once

namespace shared
{
   namespace process
   {
      //-----------------------------
      ///\brief   Class which provide stop by application itself
      //-----------------------------  
      class CSoftwareStop
      {
      public:
         //-----------------------------
         ///\brief   Set an handler on stop requested
         ///\param[in] onStopRequestedFct Handler to call when stop is requested
         ///\details The handler to call when stop is requested takes another handler, to call by app when stop is finished
         //----------------------------- 
         static void setOnStopRequestedHandler(boost::function<bool()> onStopRequestedFct);

         //-----------------------------
         ///\brief   Ask application to stop
         //----------------------------- 
         static void stop();

      private:
         static boost::function<bool()> m_onStopRequestedFct;
      };
   }
} // namespace shared::process


