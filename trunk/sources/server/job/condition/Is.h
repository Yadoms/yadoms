#pragma once
#include "ICondition.h"
#include <shared/DataContainer.h>
#include "../INotificationObserverForJobsManager.h"
#include "IConditionRoot.h"
#include "IKeywordUpdater.h"

namespace job { namespace condition
{
   //-----------------------------------------------------
   ///\brief The IS operator
   //-----------------------------------------------------
   class CIs : public ICondition, public IKeywordUpdater, public boost::enable_shared_from_this<CIs>
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] configuration Condition configuration node
      //-----------------------------------------------------
      CIs(const shared::CDataContainer& configuration);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CIs();

   protected:
      // ICondition Implementation
      virtual bool eval() const;
      virtual void registerToNotificationCenter(boost::shared_ptr<INotificationObserverForJobsManager> notificationObserver, boost::shared_ptr<IConditionRootUpdater> conditionRootNotifier);
      virtual void unregisterFromNotificationCenter(boost::shared_ptr<INotificationObserverForJobsManager> notificationObserver);
      // [END] ICondition Implementation

      // IKeywordUpdater Implementation
      virtual int getKeywordId() const;
      virtual void onKeywordStateChange(const std::string& state);
      // [END] IKeywordUpdater Implementation

   private:
      //-----------------------------------------------------
      ///\brief               Keyword ID
      //-----------------------------------------------------
      const int m_keywordId;//TODO � conserver ?

      //-----------------------------------------------------
      ///\brief               Current state
      //-----------------------------------------------------
      std::string m_state;

      //-----------------------------------------------------
      ///\brief               Expected state
      //-----------------------------------------------------
      const std::string m_expectedState;
   };
	
} } // namespace job::condition	
	
	