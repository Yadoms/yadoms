#pragma once
#include <shared/process/IEndOfProcessObserver.h>
#include "../IRuleStartErrorObserver.h"
#include "../IRuleStateHandler.h"

namespace automation
{
   namespace script
   {
      //-----------------------------------------------------
      ///\brief The script stop notifier
      //-----------------------------------------------------
      class StopNotifier : public IRuleStartErrorObserver, public shared::process::IEndOfProcessObserver
      {
      public:
         //-----------------------------------------------------
         ///\brief               Constructor
         ///\param[in] ruleStateHandler   The global state handler
         ///\param[in] ruleId    The associated rule ID
         //-----------------------------------------------------
         StopNotifier(boost::shared_ptr<IRuleStateHandler> ruleStateHandler, int ruleId);

         //-----------------------------------------------------
         ///\brief               Destructor
         //-----------------------------------------------------
         virtual ~StopNotifier();

         // IRuleStartErrorObserver Implementation
         void notifyStartError(const std::string& error) override;
         // [END] IRuleStartErrorObserver Implementation

         // IStopNotifier Implementation
         void onEndOfProcess(int returnCode, const std::string& error) override;
         // [END] IStopNotifier Implementation

      protected:
         virtual void notifyNormalStop();
         virtual void notifyError(const std::string& error);

      private:
         //-----------------------------------------------------
         ///\brief               The global rule state handler
         //-----------------------------------------------------
         boost::shared_ptr<IRuleStateHandler> m_ruleStateHandler;

         //-----------------------------------------------------
         ///\brief               The concerned rule ID
         //-----------------------------------------------------
         int m_ruleId;
      };
   }
} // namespace automation::script
