#pragma once
#include <shared/process/IApplicationStopHandler.h>

namespace shared
{
   namespace process
   {
      //-----------------------------
      ///\brief   Windows application stop handler
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
         bool m_isRunningAsService;
      };
   }
} // namespace shared::process