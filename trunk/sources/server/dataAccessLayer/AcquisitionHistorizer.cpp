#include "stdafx.h"
#include <shared/Log.h>
#include "AcquisitionHistorizer.h"
#include "database/ITransactionalProvider.h"

#include "notification/acquisition/Notification.hpp"
#include "notification/Helpers.hpp"

namespace dataAccessLayer {

	CAcquisitionHistorizer::CAcquisitionHistorizer(boost::shared_ptr<database::IDataProvider> dataProvider)
		:m_dataProvider(dataProvider)
	{
	}

	void CAcquisitionHistorizer::saveData(const int keywordId, const shared::plugin::yPluginApi::historization::IHistorizable & data)
	{
		//use ptime as variable, because saveData needs a reference
		boost::posix_time::ptime currentDate = boost::posix_time::second_clock::universal_time();

		//get current transactional engine
		boost::shared_ptr<database::ITransactionalProvider> transactionalEngine = m_dataProvider->getTransactionalEngine();

		try
		{
			//if possible create transaction
			if (transactionalEngine)
				transactionalEngine->transactionBegin();

			//save data
			saveData(keywordId, data, currentDate);

			//if possible commit transaction
			if (transactionalEngine)
				transactionalEngine->transactionCommit();
		}
		catch (std::exception &)
		{
			//if possible rollback transaction
			if (transactionalEngine)
				transactionalEngine->transactionRollback();
			throw; // rethrow exception, catch is just here to handle transaction
		}
	}

	void CAcquisitionHistorizer::saveData(std::vector<int> keywordIdVect, const std::vector<boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> > & dataVect)
	{
		//use ptime as variable, because saveData needs a reference
		boost::posix_time::ptime currentDate = boost::posix_time::second_clock::universal_time();

		//get current transactional engine
		boost::shared_ptr<database::ITransactionalProvider> transactionalEngine = m_dataProvider->getTransactionalEngine();

		try
		{
			//if possible create transaction
			if (transactionalEngine)
				transactionalEngine->transactionBegin();

			//save all data
			for (unsigned int keywordIdCount = 0; keywordIdCount < keywordIdVect.size(); ++keywordIdCount)
			{
				saveData(keywordIdVect[keywordIdCount], *dataVect[keywordIdCount], currentDate);
			}

			//if possible commit transaction
			if (transactionalEngine)
				transactionalEngine->transactionCommit();
		}
		catch (std::exception &)
		{
			//if possible rollback transaction
			if (transactionalEngine)
				transactionalEngine->transactionRollback();
			throw; // rethrow exception, catch is just here to handle transaction
		}
	}

	void CAcquisitionHistorizer::saveData(const int keywordId, const shared::plugin::yPluginApi::historization::IHistorizable & data, boost::posix_time::ptime & dataTime)
	{
		boost::shared_ptr<database::entities::CAcquisition> acq;

		//save data
		if (data.getMeasureType() == shared::plugin::yPluginApi::historization::EMeasureType::kIncrement)
			acq = m_dataProvider->getAcquisitionRequester()->incrementData(keywordId, data.formatValue(), dataTime);
		else
			acq = m_dataProvider->getAcquisitionRequester()->saveData(keywordId, data.formatValue(), dataTime);

		database::IAcquisitionRequester::LastSummaryData summaryData = m_dataProvider->getAcquisitionRequester()->saveSummaryData(keywordId, dataTime);


      //post notification
      boost::shared_ptr<notification::acquisition::CNotification> notificationData(new notification::acquisition::CNotification(acq, summaryData.get<0>(), summaryData.get<1>()));
      notification::CHelpers::postNotification(notificationData);
	}


} //namespace dataAccessLayer 
