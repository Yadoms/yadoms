#include "stdafx.h"
#include "Acquisition.h"
#include <shared/exception/NotImplemented.hpp>
#include <shared/exception/EmptyResult.hpp>
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include "database/sqlite/adapters/MultipleValueAdapter.hpp"
#include "database/sqlite/adapters/HighchartValueAdapter.hpp"
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"

namespace database {  namespace sqlite {  namespace requesters { 



   CAcquisition::CAcquisition(CSQLiteDataProvider * databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
      :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
   {
      BOOST_ASSERT(m_databaseHandler != NULL);
   }

   CAcquisition::~CAcquisition()
   {
   }


   // IAcquisitionRequester implementation
   void CAcquisition::saveData(const int keywordId, const std::string & data)
   {
      //use ptime as variable, because saveData needs a reference
      boost::posix_time::ptime currentDate = boost::posix_time::second_clock::universal_time();
      saveData(keywordId, data, currentDate);
   }


   void CAcquisition::saveData(const int keywordId, const std::string & data, boost::posix_time::ptime & dataTime)
   {
      if(m_databaseHandler->getKeywordRequester()->getKeyword(keywordId))
      {
         CQuery q;
         q.InsertInto(CAcquisitionTable::getTableName(), CAcquisitionTable::getDateColumnName(), CAcquisitionTable::getKeywordIdColumnName(), CAcquisitionTable::getValueColumnName()).
            Values(dataTime, keywordId, data);

         if(m_databaseRequester->queryStatement(q) <= 0)
            throw shared::exception::CEmptyResult("Fail to insert new data");
      }
      else
      {
         throw shared::exception::CEmptyResult("The keyword do not exists, cannot add data");
      }
   }

   void CAcquisition::removeKeywordData(const int keywordId)
   {
      if(m_databaseHandler->getKeywordRequester()->getKeyword(keywordId))
      {
         CQuery q;
         q.DeleteFrom(CAcquisitionTable::getTableName()).
            Where(CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId);   
         m_databaseRequester->queryStatement(q);

         CQuery qSummary;
         qSummary.DeleteFrom(CAcquisitionSummaryTable::getTableName()).
            Where(CAcquisitionSummaryTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId);
         m_databaseRequester->queryStatement(qSummary);
      }
      else
      {
         throw shared::exception::CEmptyResult("The keyword do not exists, cannot delete data");
      }
   }


   boost::shared_ptr< database::entities::CAcquisition > CAcquisition::getAcquisitionById(const int acqId)
   {
      CQuery qSelect;
      qSelect. Select().
         From(CAcquisitionTable::getTableName()).
         Where(CAcquisitionTable::getIdColumnName() , CQUERY_OP_EQUAL, acqId);

      database::sqlite::adapters::CAcquisitionAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CAcquisition> >(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
      {
         return adapter.getResults()[0];
      }
      else
      {
         std::string sEx = (boost::format("Cannot retrieve Acquisition Id=%1% in database") % acqId).str(); 
         throw shared::exception::CEmptyResult(sEx);
      }
   }


   boost::shared_ptr< database::entities::CAcquisition > CAcquisition::getAcquisitionByKeywordAndDate(const int keywordId, boost::posix_time::ptime time)
   {
      CQuery qSelect;
      qSelect.Select().
         From(CAcquisitionTable::getTableName()).
         Where(CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
         And(CAcquisitionTable::getDateColumnName(), CQUERY_OP_EQUAL, time);

      database::sqlite::adapters::CAcquisitionAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CAcquisition> >(&adapter, qSelect);
      if (adapter.getResults().size() >= 1)
      {
         return adapter.getResults()[0];
      }
      else
      {
         std::string sEx = (boost::format("Cannot retrieve acquisition for KeywordId=%1% and date=%2%  in database") % keywordId % time).str();
         throw shared::exception::CEmptyResult(sEx);
      }
   }


   boost::shared_ptr< database::entities::CAcquisition > CAcquisition::getKeywordLastData(const int keywordId)
   {
      CQuery qSelect;
      qSelect. Select().
               From(CAcquisitionTable::getTableName()).
               Where(CAcquisitionTable::getKeywordIdColumnName() , CQUERY_OP_EQUAL, keywordId).
               OrderBy(CAcquisitionTable::getDateColumnName(), CQUERY_ORDER_DESC).
               Limit(1);

      database::sqlite::adapters::CAcquisitionAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CAcquisition> >(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
      {
         return adapter.getResults()[0];
      }
      else
      {
         std::string sEx = (boost::format("Cannot retrieve any acquisition for the keyword id=%1% in database") % keywordId).str(); 
         throw shared::exception::CEmptyResult(sEx);
      }
   }

   std::vector< boost::tuple<boost::posix_time::ptime, std::string>  > CAcquisition::getKeywordData(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
   {
      CQuery qSelect;
      qSelect. Select(CAcquisitionTable::getDateColumnName(), CAcquisitionTable::getValueColumnName()).
         From(CAcquisitionTable::getTableName()).
         Where(CAcquisitionTable::getKeywordIdColumnName() , CQUERY_OP_EQUAL, keywordId);

      if(!timeFrom.is_not_a_date_time())
      {
         qSelect.And(CAcquisitionTable::getDateColumnName(), CQUERY_OP_SUP_EQUAL, timeFrom);
         if(!timeTo.is_not_a_date_time())
         {
            qSelect.And(CAcquisitionTable::getDateColumnName(), CQUERY_OP_INF_EQUAL, timeTo);
         }
      }

      qSelect.OrderBy(CAcquisitionTable::getDateColumnName());

      database::sqlite::adapters::CMultipleValueAdapter<boost::posix_time::ptime, std::string> mva;
      m_databaseRequester->queryEntities(&mva, qSelect);
      return mva.getResults();
   }

   std::string CAcquisition::getKeywordHighchartData(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
   {
      //this query is optimized to get only one field to read
      //the output data is a single column (without brackets):  ["dateiso",value]
      CQuery qSelect;
      qSelect.Clear().Select("(strftime('%s', isodate(" + CAcquisitionTable::getDateColumnName() + "))*1000) ||','|| " + CAcquisitionTable::getValueColumnName()).
         From(CAcquisitionTable::getTableName()).
         Where(CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId);

      if (!timeFrom.is_not_a_date_time())
      {
         qSelect.And(CAcquisitionTable::getDateColumnName(), CQUERY_OP_SUP_EQUAL, timeFrom);
         if (!timeTo.is_not_a_date_time())
         {
            qSelect.And(CAcquisitionTable::getDateColumnName(), CQUERY_OP_INF_EQUAL, timeTo);
         }
      }

      qSelect.OrderBy(CAcquisitionTable::getDateColumnName());
      database::sqlite::adapters::CHighchartValueAdapter mva;
      m_databaseRequester->queryEntities(&mva, qSelect);
      return mva.getRawResults();
   }

   std::string CAcquisition::getKeywordHighchartDataByDay(const database::entities::EAcquisitionSummaryType & type, int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
   {
      //this query is optimized to get only one field to read
      //the output data is a single column (without brackets):  ["dateiso",value]
      CQuery qSelect;
      qSelect.Clear().Select("(strftime('%s', isodate(" + CAcquisitionSummaryTable::getDateColumnName() + "))*1000) ||','|| " + CAcquisitionSummaryTable::getMeanColumnName()).
         From(CAcquisitionSummaryTable::getTableName()).
         Where(CAcquisitionSummaryTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
         And(CAcquisitionSummaryTable::getTypeColumnName(), CQUERY_OP_EQUAL, type);

      if (!timeFrom.is_not_a_date_time())
      {
         qSelect.And(CAcquisitionSummaryTable::getDateColumnName(), CQUERY_OP_SUP_EQUAL, timeFrom);
         if (!timeTo.is_not_a_date_time())
         {
            qSelect.And(CAcquisitionSummaryTable::getDateColumnName(), CQUERY_OP_INF_EQUAL, timeTo);
         }
      }

      qSelect.OrderBy(CAcquisitionSummaryTable::getDateColumnName());
      database::sqlite::adapters::CHighchartValueAdapter mva;
      m_databaseRequester->queryEntities(&mva, qSelect);
      return mva.getRawResults();

   }


   std::string CAcquisition::getKeywordHighchartDataByDay(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
   {
      return getKeywordHighchartDataByDay(database::entities::EAcquisitionSummaryType::kDay, keywordId, timeFrom, timeTo);
   }

   std::string CAcquisition::getKeywordHighchartDataByHour(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
   {
      return getKeywordHighchartDataByDay(database::entities::EAcquisitionSummaryType::kHour, keywordId, timeFrom, timeTo);
   }

   void CAcquisition::saveSummaryData(const database::entities::EAcquisitionSummaryType type, const int keywordId, boost::posix_time::ptime & dataTime, double mean, double min, double max)
   {
      if (m_databaseHandler->getKeywordRequester()->getKeyword(keywordId))
      {
         CQuery q;
         q.InsertInto(CAcquisitionSummaryTable::getTableName(), CAcquisitionSummaryTable::getTypeColumnName(), CAcquisitionSummaryTable::getKeywordIdColumnName(), CAcquisitionSummaryTable::getDateColumnName(), CAcquisitionSummaryTable::getMeanColumnName(), CAcquisitionSummaryTable::getMinColumnName(), CAcquisitionSummaryTable::getMaxColumnName()).
            Values(type, keywordId, dataTime, keywordId, mean, min, max);

         if (m_databaseRequester->queryStatement(q) <= 0)
            throw shared::exception::CEmptyResult("Fail to insert new summary data");
      }
      else
      {
         throw shared::exception::CEmptyResult("The keyword do not exists, cannot add summary data");
      }
   }


   // [END] IAcquisitionRequester implementation


} //namespace requesters
} //namespace sqlite
} //namespace database 

