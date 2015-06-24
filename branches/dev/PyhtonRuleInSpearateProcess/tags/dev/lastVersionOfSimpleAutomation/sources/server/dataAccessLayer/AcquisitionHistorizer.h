#pragma once
#include "IAcquisitionHistorizer.h"
#include "database/IAcquisitionRequester.h"
#include <shared/notification/NotificationCenter.h>

namespace dataAccessLayer {

   class CAcquisitionHistorizer : public IAcquisitionHistorizer
   {
   public:
      CAcquisitionHistorizer(boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester, boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter);

      virtual void saveData(const int keywordId, const shared::plugin::yPluginApi::historization::IHistorizable & data);
      virtual void saveData(const int keywordId, const shared::plugin::yPluginApi::historization::IHistorizable & data, boost::posix_time::ptime & dataTime);

   private:

   private:
      boost::shared_ptr<database::IAcquisitionRequester> m_acquisitionRequester;
      boost::shared_ptr<shared::notification::CNotificationCenter> m_notificationCenter;
   };
 
} //namespace dataAccessLayer 
