#pragma once
#include <shared/script/IStopNotifier.h>
#include "../IRuleStateHandler.h"

namespace automation { namespace script
{
   //-----------------------------------------------------
   ///\brief The script stop notifier
   //-----------------------------------------------------
   class StopNotifier : public shared::script::IStopNotifier
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

      // IStopNotifier Implementation
      virtual void notifyNormalStop();
      virtual void notifyError(const std::string& error);
      virtual void notifyStartError(const std::string& error);
      // [END] IStopNotifier Implementation

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

} } // namespace automation::script
