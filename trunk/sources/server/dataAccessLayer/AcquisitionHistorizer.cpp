#include "stdafx.h"
#include "AcquisitionHistorizer.h"
#include "notifications/AsyncNotificationCenter.h"
#include "notifications/NewAcquisitionNotification.h"

namespace dataAccessLayer {

   CAcquisitionHistorizer::CAcquisitionHistorizer(boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester)
      :m_acquisitionRequester(acquisitionRequester)
      {
      }

      void CAcquisitionHistorizer::saveData(const int keywordId, const shared::plugin::yadomsApi::historization::IHistorizable & data)
      {
         //use ptime as variable, because saveData needs a reference
         boost::posix_time::ptime currentDate = boost::posix_time::second_clock::universal_time();
         saveData(keywordId, data, currentDate);
      }

      void CAcquisitionHistorizer::saveData(const int keywordId, const shared::plugin::yadomsApi::historization::IHistorizable & data, boost::posix_time::ptime & dataTime)
      {
         //save data
         if(data.getMeasureType() == shared::plugin::yadomsApi::historization::EMeasureType::kIncrement)
            m_acquisitionRequester->incrementData(keywordId, data.formatValue(), dataTime);
         else
            m_acquisitionRequester->saveData(keywordId, data.formatValue(), dataTime);

         m_acquisitionRequester->saveSummaryData(keywordId, dataTime);

         //post notification
         try
         {
            //get inserted raw data, and store it into container
            boost::shared_ptr< database::entities::CAcquisition > acq = m_acquisitionRequester->getAcquisitionByKeywordAndDate(keywordId, dataTime);

            boost::shared_ptr< database::entities::CAcquisitionSummary > acqDay;
            std::vector< boost::shared_ptr< database::entities::CAcquisitionSummary > > dayData = m_acquisitionRequester->getKeywordDataByDay(keywordId, dataTime, dataTime);
            if (dayData.size() > 0)
            {
               acqDay = dayData[0];
            }

            boost::shared_ptr< database::entities::CAcquisitionSummary > acqHour;
            std::vector< boost::shared_ptr< database::entities::CAcquisitionSummary > > hourData = m_acquisitionRequester->getKeywordDataByHour(keywordId, dataTime, dataTime);
            if (hourData.size() > 0)
            {
               acqHour = hourData[0];
            }

            //construct notification container
            boost::shared_ptr< notifications::CNewAcquisitionNotification > notificationData(new notifications::CNewAcquisitionNotification(acq, acqDay, acqHour));

            notifications::CAsyncNotificationCenter::get()->postNotification(notificationData);
         }
         catch (shared::exception::CException & ex)
         {
            YADOMS_LOG(error) << "Fail to retreive new acquisition : " << ex.what();
         }
      }


} //namespace dataAccessLayer 
