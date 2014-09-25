#include "stdafx.h"
#include "AcquisitionHistorizer.h"
#include "notifications/AsyncNotificationCenter.h"
#include "notifications/NewAcquisitionNotification.h"

namespace dataAccessLayer {

      CAcquisitionHistorizer::CAcquisitionHistorizer(boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester)
         :m_acquisitionRequester(acquisitionRequester)
      {
      }

      void CAcquisitionHistorizer::saveData(const int keywordId, const std::string & data)
      {
         //use ptime as variable, because saveData needs a reference
         boost::posix_time::ptime currentDate = boost::posix_time::second_clock::universal_time();
         saveData(keywordId, data, currentDate);
      }

      void CAcquisitionHistorizer::saveData(const int keywordId, const std::string & data, boost::posix_time::ptime & dataTime)
      {
         m_acquisitionRequester->saveData(keywordId, data, dataTime);

         try
         {
            boost::shared_ptr< database::entities::CAcquisition > acq = m_acquisitionRequester->getAcquisitionByKeywordAndDate(keywordId, dataTime);
            notifications::CAsyncNotificationCenter::get()->postNotification(acq);
         }
         catch (shared::exception::CException & ex)
         {
            YADOMS_LOG(error) << "Fail to retreive new acquisition : " << ex.what();
         }
      }

} //namespace dataAccessLayer 
