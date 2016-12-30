#pragma once
#include "IRule.h"
#include "IManager.h"
#include "server/database/entities/Entities.h"
#include "IRuleStateHandler.h"
#include <shared/shared/process/IProcess.h>

namespace automation
{
   //-----------------------------------------------------
   ///\brief A rule
   //-----------------------------------------------------
   class CRule : public IRule
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] ruleData           Rule data (ID, name, conditions, actions...)
      ///\param[in] scriptManager      The script manager
      ///\param[in] ruleStateHandler   The rule state handler
      //-----------------------------------------------------
      CRule(boost::shared_ptr<const database::entities::CRule> ruleData,
            boost::shared_ptr<IManager> scriptManager,
            boost::shared_ptr<IRuleStateHandler> ruleStateHandler);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CRule();

      // IRule Implementation
      void requestStop() override;
      // [END] IRule Implementation

   protected:
      //-----------------------------------------------------
      ///\brief               Start the rule
      //-----------------------------------------------------
      void start();

   private:
      //-----------------------------------------------------
      ///\brief               Minimum rule duration
      //-----------------------------------------------------
      static const boost::chrono::milliseconds m_MinRuleDuration;

      //-----------------------------------------------------
      ///\brief               Rule data
      //-----------------------------------------------------
      boost::shared_ptr<const database::entities::CRule> m_ruleData;

      //-----------------------------------------------------
      ///\brief               The script manager
      //-----------------------------------------------------
      boost::shared_ptr<IManager> m_scriptManager;

      //-----------------------------------------------------
      ///\brief               The script process
      //-----------------------------------------------------
      boost::shared_ptr<shared::process::IProcess> m_process;

      //-----------------------------------------------------
      ///\brief               The rule state handler
      //-----------------------------------------------------
      boost::shared_ptr<IRuleStateHandler> m_ruleStateHandler;
   };
} // namespace automation	


