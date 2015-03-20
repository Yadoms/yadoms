#pragma once
#include "IAcquisitionHistorizer.h"
#include "database/IDataProvider.h"
#include <shared/notification/NotificationCenter.h>

namespace dataAccessLayer
{

   class CAcquisitionHistorizer : public IAcquisitionHistorizer
   {
   public:
      CAcquisitionHistorizer(boost::shared_ptr<database::IDataProvider> dataProvider, boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter);

      virtual void saveData(const int keywordId, const shared::plugin::yPluginApi::historization::IHistorizable & data);
      virtual void saveData(std::vector<int> KeywordIdVect, std::vector<boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> > & dataVect);
      virtual void saveData(const int keywordId, const shared::plugin::yPluginApi::historization::IHistorizable & data, boost::posix_time::ptime & dataTime);

   private:
      boost::shared_ptr<database::IDataProvider> m_dataProvider;
      boost::shared_ptr<shared::notification::CNotificationCenter> m_notificationCenter;
   };
 
} //namespace dataAccessLayer 
