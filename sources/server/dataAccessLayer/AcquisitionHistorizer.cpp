#include "stdafx.h"
#include <shared/currentTime/Provider.h>
#include "AcquisitionHistorizer.h"
#include "database/ITransactionalProvider.h"

#include "notification/acquisition/Notification.hpp"
#include "notification/summary/Notification.hpp"
#include "notification/Helpers.hpp"
#include "observers/AcquisitionObservers.h"

namespace dataAccessLayer
{
   CAcquisitionHistorizer::CAcquisitionHistorizer(boost::shared_ptr<database::IDataProvider> dataProvider)
      : m_dataProvider(dataProvider),
        m_acquisitionObservers(boost::make_shared<observers::CAcquisitionObservers>())
   {
   }

   void CAcquisitionHistorizer::saveData(int keywordId,
                                         const shared::plugin::yPluginApi::historization::IHistorizable& data)
   {
      //use ptime as variable, because saveData needs a reference
      auto currentDate = shared::currentTime::Provider().now();

      //get current transactional engine
      const auto transactionalEngine = m_dataProvider->getTransactionalEngine();

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
      catch (std::exception&)
      {
         //if possible rollback transaction
         if (transactionalEngine)
            transactionalEngine->transactionRollback();
         throw; // rethrow exception, catch is just here to handle transaction
      }
   }

   void CAcquisitionHistorizer::saveData(std::vector<int> keywordIdVect,
                                         const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>&
                                         dataVect)
   {
      //use ptime as variable, because saveData needs a reference
      auto currentDate = shared::currentTime::Provider().now();

      //get current transactional engine
      const auto transactionalEngine = m_dataProvider->getTransactionalEngine();

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
      catch (std::exception&)
      {
         //if possible rollback transaction
         if (transactionalEngine)
            transactionalEngine->transactionRollback();
         throw; // rethrow exception, catch is just here to handle transaction
      }
   }

   void CAcquisitionHistorizer::saveData(int keywordId,
                                         const shared::plugin::yPluginApi::historization::IHistorizable& data,
                                         boost::posix_time::ptime& dataTime)
   {
      boost::shared_ptr<database::entities::CAcquisition> acq;

      //save data
      if (data.getMeasureType() == shared::plugin::yPluginApi::EMeasureType::kIncrement)
         acq = m_dataProvider->getAcquisitionRequester()->incrementData(keywordId, data.formatValue(), dataTime);
      else
         acq = m_dataProvider->getAcquisitionRequester()->saveData(keywordId, data.formatValue(), dataTime);

      if (acq)
      {
         std::vector<boost::shared_ptr<database::entities::CAcquisitionSummary>> acquisitionSummary;

         if (data.getHistoryDepth() != shared::plugin::yPluginApi::EHistoryDepth::kNoHistory)
         {
            //only update summary data if already exists
            //if not exists it will be created by SQLiteSummaryDataTask

            if (m_dataProvider->getAcquisitionRequester()->summaryDataExists(keywordId, database::entities::EAcquisitionSummaryType::kHour, dataTime))
               acquisitionSummary.push_back(
                  m_dataProvider->getAcquisitionRequester()->saveSummaryData(keywordId, database::entities::EAcquisitionSummaryType::kHour,
                                                                             dataTime));
            if (m_dataProvider->getAcquisitionRequester()->summaryDataExists(keywordId, database::entities::EAcquisitionSummaryType::kDay, dataTime))
               acquisitionSummary.push_back(
                  m_dataProvider->getAcquisitionRequester()->saveSummaryData(keywordId, database::entities::EAcquisitionSummaryType::kDay, dataTime));
            if (m_dataProvider->getAcquisitionRequester()->
                                summaryDataExists(keywordId, database::entities::EAcquisitionSummaryType::kMonth, dataTime))
               acquisitionSummary.push_back(
                  m_dataProvider->getAcquisitionRequester()->
                                  saveSummaryData(keywordId, database::entities::EAcquisitionSummaryType::kMonth, dataTime));
            if (m_dataProvider->getAcquisitionRequester()->summaryDataExists(keywordId, database::entities::EAcquisitionSummaryType::kYear, dataTime))
               acquisitionSummary.push_back(
                  m_dataProvider->getAcquisitionRequester()->saveSummaryData(keywordId, database::entities::EAcquisitionSummaryType::kYear,
                                                                             dataTime));
         }

         // Post notifications
         postNotification(acq);
         if (!acquisitionSummary.empty())
            postNotification(acquisitionSummary);
      }
   }

   boost::shared_ptr<observers::IAcquisitionObservers> CAcquisitionHistorizer::acquisitionObservers()
   {
      return m_acquisitionObservers;
   }

   void CAcquisitionHistorizer::postNotification(boost::shared_ptr<database::entities::CAcquisition> acquisition) const
   {
      m_acquisitionObservers->onNewAcquisition(acquisition);

      // TODO virer (utilise l'obsolète CNotificationCenter)
      const auto notificationData = boost::make_shared<notification::acquisition::CNotification>(acquisition);
      notification::CHelpers::postNotification(notificationData);
   }

   void CAcquisitionHistorizer::postNotification(const std::vector<boost::shared_ptr<database::entities::CAcquisitionSummary>>& acquisitionSummary)
   {
      //TODO faire pareil que pour m_acquisitionObservers
      //for (auto& acquisitionObserver : m_acquisitionSummaryObservers)
      //   acquisitionObserver->onNewAcquisition(acquisition);

      // TODO virer (utilise l'obsolète CNotificationCenter)
      const auto notificationDataSummary = boost::make_shared<notification::summary::CNotification>(acquisitionSummary);
      notification::CHelpers::postNotification(notificationDataSummary);
   }
} //namespace dataAccessLayer 
