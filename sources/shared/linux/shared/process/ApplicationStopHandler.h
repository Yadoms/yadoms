#pragma once
#include <shared/process/IApplicationStopHandler.h>

namespace shared
{
   namespace process
   {
      //-----------------------------
      ///\brief   Linux application stop handler
      //-----------------------------  
      class CApplicationStopHandler : IApplicationStopHandler
      {
      public:
         explicit CApplicationStopHandler(bool isRunningAsService);

         virtual ~CApplicationStopHandler();

         // IApplicationStopHandler implementation
         void setApplicationStopHandler(boost::function<bool()> onStopRequestedFct) override;
         // [END] IApplicationStopHandler implementation

      private:
         static boost::function<bool()> m_onStopRequestedFct;

         static void crashHandler(int signal);
         static void stopHandler(int signal);
      };
   }
} // namespace shared::process
