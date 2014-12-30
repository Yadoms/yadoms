#include "stdafx.h"
#include "AcquisitionHistorizer.h"
#include "notifications/NewAcquisitionNotification.h"

namespace dataAccessLayer {

   CAcquisitionHistorizer::CAcquisitionHistorizer(boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester,
      boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter)
      :m_acquisitionRequester(acquisitionRequester), m_notificationCenter(notificationCenter)
      {
      }

      void CAcquisitionHistorizer::saveData(const int keywordId, const shared::plugin::yPluginApi::historization::IHistorizable & data)
      {
         //use ptime as variable, because saveData needs a reference
         boost::posix_time::ptime currentDate = boost::posix_time::second_clock::universal_time();
         saveData(keywordId, data, currentDate);
      }

      void CAcquisitionHistorizer::saveData(const int keywordId, const shared::plugin::yPluginApi::historization::IHistorizable & data, boost::posix_time::ptime & dataTime)
      {
         boost::shared_ptr<const database::entities::CAcquisition> acq;

         //save data
         if(data.getMeasureType() == shared::plugin::yPluginApi::historization::EMeasureType::kIncrement)
            acq = m_acquisitionRequester->incrementData(keywordId, data.formatValue(), dataTime);
         else
            acq = m_acquisitionRequester->saveData(keywordId, data.formatValue(), dataTime);

         database::IAcquisitionRequester::LastSummaryData summaryData = m_acquisitionRequester->saveSummaryData(keywordId, dataTime);

         //post notification
         try
         {
            boost::shared_ptr< notifications::CNewAcquisitionNotification > notificationData(new notifications::CNewAcquisitionNotification(acq, summaryData.get<0>(), summaryData.get<1>()));
            m_notificationCenter->postNotification(notificationData);
         }
         catch (shared::exception::CException & ex)
         {
            YADOMS_LOG(error) << "Fail to retreive new acquisition : " << ex.what();
         }
      }


} //namespace dataAccessLayer 
