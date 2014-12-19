#pragma once
#include "ICondition.h"
#include <shared/DataContainer.h>
#include "../INotificationObserverForJobsManager.h"
#include "IConditionRoot.h"
#include "IKeywordUpdater.h"
#include "../../database/IAcquisitionRequester.h"

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
      ///\param[in] dbAcquisitionRequester  Database acquisition requester
      ///\throw shared::exception::CEmptyResult if associated keyword is not found
      //-----------------------------------------------------
      CIs(const shared::CDataContainer& configuration, boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester);

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
      const int m_keywordId;

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
	
	