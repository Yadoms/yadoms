#pragma once
#include "ICondition.h"
#include <shared/shared/DataContainer.h>
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
      ///\param[in] conditionRoot The condition root to notify when changes arrives on the keyword
      ///\param[in] notificationObserver  The notification observer
      //-----------------------------------------------------
      CIs(const shared::CDataContainer& configuration, IConditionRoot& conditionRoot, boost::shared_ptr<INotificationObserverForJobsManager> notificationObserver);//TODO vérfier si tous les paramètres sont encore utiles

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CIs();

   protected:
      // ICondition Implementation
      virtual bool eval() const;
      // [END] ICondition Implementation

      // IKeywordUpdater Implementation
      virtual int getKeywordId() const;
      virtual void onKeywordStateChange(const std::string& state);
      // [END] IKeywordUpdater Implementation

   private:
      //-----------------------------------------------------
      ///\brief               Keyword ID
      //-----------------------------------------------------
      const int m_keywordId;//TODO à conserver ?

      //-----------------------------------------------------
      ///\brief               Current state
      //-----------------------------------------------------
      std::string m_state;

      //-----------------------------------------------------
      ///\brief               Expected state
      //-----------------------------------------------------
      const std::string m_expectedState;

      //-----------------------------------------------------
      ///\brief               The notification observer to register with
      //-----------------------------------------------------
      boost::shared_ptr<INotificationObserverForJobsManager> m_notificationObserver;
   };
	
} } // namespace job::condition	
	
	