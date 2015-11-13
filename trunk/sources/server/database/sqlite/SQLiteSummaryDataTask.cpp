#include "stdafx.h"

#include "SQLiteSummaryDataTask.h"
#include "database/entities/Entities.h"
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include "database/sqlite/adapters/MultipleValueAdapter.hpp"
#include "database/sqlite/adapters/HighchartValueAdapter.hpp"
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"
#include <shared/plugin/yPluginApi/KeywordDataType.h>
#include <shared/currentTime/Provider.h>

#include "notification/acquisition/Notification.hpp"
#include "notification/summary/Notification.hpp"
#include "notification/Helpers.hpp"

namespace database { 
namespace sqlite { 


   CSQLiteSummaryDataTask::CSQLiteSummaryDataTask(CSQLiteDataProvider * databaseProvider)
      :m_databaseProvider(databaseProvider), m_firstRun(true)
   {
   }

   CSQLiteSummaryDataTask::~CSQLiteSummaryDataTask()
   {
   }

   void CSQLiteSummaryDataTask::run()
   {
      YADOMS_LOG_CONFIGURE("Summary computing task");

      YADOMS_LOG(information) << "Start computing summary values";

     
      if (m_firstRun)
      {
         m_firstRun = false;
         executeFirstRunPass();
      }
      else
      {
         executeCommonPass();
      }
   }


   void CSQLiteSummaryDataTask::executeFirstRunPass()
   {
      /*

      Algo
      ===============

      Contexte
      ------------
      Cette m�thode est lanc�e une seule fois au lancement de l'application

      On r�cup�re la derni�re donn�e (brute) de chaque keyword

      Pour chaque keywords de type numeric
         si la date d'enregistrement de la derni�re valeur est d'une heure diff�rente de maintenant (comparaison date+heure) et que la donn�e r�sum�e HEURE n'existe pas
         alors on recalcule la donn�e r�sum�e HEURE

         si la date d'enregistrement de la derni�re valeur est diff�rente de maintenant (comparaison date) et que la donn�e r�sum�e JOUR n'existe pas
         alors on recalcule la donn�e r�sum�e JOUR
      */


      //check all keyword last date value
      //foreach
      //    if SummaryHour don't exist, then compute it
      //    if SummaryDay don't exist, then compute it
      std::vector<boost::shared_ptr<entities::CKeyword> > keywords = m_databaseProvider->getKeywordRequester()->getAllKeywords();
      std::vector<boost::shared_ptr<entities::CKeyword> >::iterator i;
      for (i = keywords.begin(); i != keywords.end(); ++i)
      {
         if (i->get()->Type() == shared::plugin::yPluginApi::EKeywordDataType::kNumeric)
         {
            boost::shared_ptr<entities::CAcquisition> lastData = m_databaseProvider->getAcquisitionRequester()->getKeywordLastData(i->get()->Id);
            
            //if current day
            //    if NOT same hour
            //       compute HOUR
            //else
            //    compute hour
            //    compute day


            tm acqDate = boost::posix_time::to_tm(lastData->Date());
            tm now = boost::posix_time::to_tm(boost::posix_time::second_clock::local_time());
            
            bool computeHourValue = false, computeDayValue = false;

            if (acqDate.tm_year == now.tm_year && acqDate.tm_yday == now.tm_yday)
            {
               //same day, do not compute day summary data
               if (acqDate.tm_hour != now.tm_hour)
               {
                  //the hour have changed, compute hour summary data
                  computeHourValue = true;
               }
            }
            else
            {
               //another day
               computeDayValue = true;
               computeHourValue = true;
            }


            if (computeHourValue)
            {
               if (!m_databaseProvider->getAcquisitionRequester()->summaryDataExists(i->get()->Id(), database::entities::EAcquisitionSummaryType::kHour, lastData->Date()))
               {
                  YADOMS_LOG(information) << "    Computing HOUR : " << i->get()->FriendlyName() << " @ " << lastData->Date();
                  m_databaseProvider->getAcquisitionRequester()->saveSummaryData(i->get()->Id(), database::entities::EAcquisitionSummaryType::kHour, lastData->Date());
               }
            }

            if (computeDayValue)
            {
               if (!m_databaseProvider->getAcquisitionRequester()->summaryDataExists(i->get()->Id(), database::entities::EAcquisitionSummaryType::kDay, lastData->Date()))
               {
                  YADOMS_LOG(information) << "    Computing DAY : " << i->get()->FriendlyName() << " @ " << lastData->Date();
                  m_databaseProvider->getAcquisitionRequester()->saveSummaryData(i->get()->Id(), database::entities::EAcquisitionSummaryType::kDay, lastData->Date());
               }
            }
         }
      }
   }

   void CSQLiteSummaryDataTask::executeCommonPass()
   {
      /*

      Algo
      ===============

      Contexte
      ------------
      Cette m�thode est lanc�e chaque heure

      Pour tous les keywords ayant �mis � l'heure pr�c�dente, on lance le calcul des valeurs r�sum�es (si la valeur n'existe pas)
      Pour tous les keywords ayant �mis le jour pr�c�dent, on lance le calcul des valeurs r�sum�es (si la valeur n'existe pas)

      */
      boost::posix_time::ptime now = shared::currentTime::Provider::now();
      boost::posix_time::time_duration oneHour = boost::posix_time::hours(1);

      boost::posix_time::ptime currentHour(now.date(), boost::posix_time::hours(now.time_of_day().hours()));
      boost::posix_time::ptime previousHour = currentHour - oneHour;

      std::vector<int> keywordToTreat;
      std::vector< boost::shared_ptr< database::entities::CAcquisitionSummary> > summaryAcquisitions;

      m_databaseProvider->getAcquisitionRequester()->getKeywordsHavingDate(previousHour, currentHour, keywordToTreat);
      for (std::vector<int>::iterator i = keywordToTreat.begin(); i != keywordToTreat.end(); ++i)
      {
         if (!m_databaseProvider->getAcquisitionRequester()->summaryDataExists(*i, database::entities::EAcquisitionSummaryType::kHour, previousHour))
         {
            YADOMS_LOG(information) << "    Computing HOUR : " << *i << " @ " << previousHour;
            boost::shared_ptr< database::entities::CAcquisitionSummary> insertedValue = m_databaseProvider->getAcquisitionRequester()->saveSummaryData(*i, database::entities::EAcquisitionSummaryType::kHour, previousHour);
            summaryAcquisitions.push_back(insertedValue);
         }
      }

      //check if day have changed
      if (currentHour.date() != previousHour.date())
      {

         boost::posix_time::time_duration oneDay = boost::posix_time::hours(24);
         boost::posix_time::ptime currentDay(now.date());
         boost::posix_time::ptime previousDay = currentDay - oneDay;

         keywordToTreat.clear();
         m_databaseProvider->getAcquisitionRequester()->getKeywordsHavingDate(previousDay, currentDay, keywordToTreat);
         for (std::vector<int>::iterator i = keywordToTreat.begin(); i != keywordToTreat.end(); ++i)
         {
            if (!m_databaseProvider->getAcquisitionRequester()->summaryDataExists(*i, database::entities::EAcquisitionSummaryType::kDay, previousDay))
            {
               YADOMS_LOG(information) << "    Computing DAY : " << *i << " @ " << previousDay;
               boost::shared_ptr< database::entities::CAcquisitionSummary> insertedValue = m_databaseProvider->getAcquisitionRequester()->saveSummaryData(*i, database::entities::EAcquisitionSummaryType::kDay, previousDay);
               summaryAcquisitions.push_back(insertedValue);

            }
         }
      }

      //post notification
      if (!summaryAcquisitions.empty())
      {
         boost::shared_ptr<notification::summary::CNotification> notificationData(new notification::summary::CNotification(summaryAcquisitions));
         notification::CHelpers::postNotification(notificationData);
      }
   }


} //namespace sqlite
} //namespace database 

