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



   std::vector< boost::tuple<boost::posix_time::ptime, std::string, std::string, std::string> > CAcquisition::getKeywordDataByDay(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
   {
      return getKeywordSummaryDataByType(database::entities::EAcquisitionSummaryType::kDay, keywordId, timeFrom, timeTo);
   }



   std::vector< boost::tuple<boost::posix_time::ptime, std::string, std::string, std::string> > CAcquisition::getKeywordDataByHour(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
   {
      return getKeywordSummaryDataByType(database::entities::EAcquisitionSummaryType::kHour, keywordId, timeFrom, timeTo);
   }




   std::vector< boost::tuple<boost::posix_time::ptime, std::string, std::string, std::string>  > CAcquisition::getKeywordSummaryDataByType(const database::entities::EAcquisitionSummaryType & type, int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
   {
      CQuery qSelect;
      qSelect.Clear().Select(CAcquisitionSummaryTable::getDateColumnName(), CAcquisitionSummaryTable::getAvgColumnName(), CAcquisitionSummaryTable::getMinColumnName(), CAcquisitionSummaryTable::getMaxColumnName()).
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

      if (type == database::entities::EAcquisitionSummaryType::kDay)
         qSelect.Limit(2000); //more than 5 years
      else
         qSelect.Limit(200); // more than 8 days

      database::sqlite::adapters::CMultipleValueAdapter<boost::posix_time::ptime, std::string, std::string, std::string> mva;
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

   std::string CAcquisition::getKeywordHighchartDataByType(const database::entities::EAcquisitionSummaryType & type, int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
   {
      //this query is optimized to get only one field to read
      //the output data is a single column (without brackets):  ["dateiso",value]
      CQuery qSelect;
      qSelect.Clear().Select("(strftime('%s', isodate(" + CAcquisitionSummaryTable::getDateColumnName() + "))*1000) ||','|| " + CAcquisitionSummaryTable::getAvgColumnName()).
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
      return getKeywordHighchartDataByType(database::entities::EAcquisitionSummaryType::kDay, keywordId, timeFrom, timeTo);
   }

   std::string CAcquisition::getKeywordHighchartDataByHour(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
   {
      return getKeywordHighchartDataByType(database::entities::EAcquisitionSummaryType::kHour, keywordId, timeFrom, timeTo);
   }

   void CAcquisition::saveSummaryData(const int keywordId, boost::posix_time::ptime & dataTime)
   {
      /* 
      INSERT OR REPLACE INTO AcquisitionSummary (type, date, keywordId, mean, min, max)
      SELECT "Hour", "startDate", 38, avg(cast(value as real)), min(cast(value as real)), max(cast(value as real))
      FROM Acquisition acq
      where acq.keywordId = 38
      and acq.date>= "startDate"
      and acq.date<= "endDate"
      
      */

      if (m_databaseHandler->getKeywordRequester()->getKeyword(keywordId))
      {
         //just compute good dates
         //hourDate : is the start of the hour (current day) => minutes and seconds set to 0
         //hourDateEnd : is the end of the hour (current day) => minutes and seconds set to 59
         //dayDate : is the start of the day : current day, with hour, minte and second set to 0
         //dayDateEnd : is the end of the day : current day, with hour set to 23, minutes and seconds at 59
         tm pt_tm = boost::posix_time::to_tm(dataTime);
         boost::posix_time::ptime hourDate(dataTime.date(), boost::posix_time::hours(pt_tm.tm_hour));
         boost::posix_time::ptime hourDateEnd(dataTime.date(), boost::posix_time::hours(pt_tm.tm_hour) + boost::posix_time::minutes(59) + boost::posix_time::seconds(59));
         boost::posix_time::ptime dayDate(dataTime.date());
         boost::posix_time::ptime dayDateEnd(dataTime.date(), boost::posix_time::hours(23) + boost::posix_time::minutes(59) + boost::posix_time::seconds(59));

         //set a variable for the type : hour
         database::entities::EAcquisitionSummaryType curType = database::entities::EAcquisitionSummaryType::kHour;
         CQuery q;
         q.InsertOrReplaceInto(CAcquisitionSummaryTable::getTableName(), CAcquisitionSummaryTable::getTypeColumnName(), CAcquisitionSummaryTable::getDateColumnName(), CAcquisitionSummaryTable::getKeywordIdColumnName(), CAcquisitionSummaryTable::getAvgColumnName(), CAcquisitionSummaryTable::getMinColumnName(), CAcquisitionSummaryTable::getMaxColumnName()).
            Select(CQueryValue(curType.getAsString()).str(), CQueryValue(hourDate).str(), CQueryValue(keywordId).str(), "avg(cast(" + CAcquisitionTable::getValueColumnName() + " as real))", "min(cast(" + CAcquisitionTable::getValueColumnName() + " as real))", "max(cast(" + CAcquisitionTable::getValueColumnName() + " as real))").
            From(CAcquisitionTable::getTableName() + " as acq").
            Where("acq." + CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
            And("acq." + CAcquisitionTable::getDateColumnName(), CQUERY_OP_SUP_EQUAL, hourDate).
            And("acq." + CAcquisitionTable::getDateColumnName(), CQUERY_OP_INF_EQUAL, hourDateEnd);

         if (m_databaseRequester->queryStatement(q) <= 0)
            throw shared::exception::CEmptyResult("Fail to insert new summary hour data");

         //set a variable for the type : day
         curType = database::entities::EAcquisitionSummaryType::kDay;
         q.Clear().
          InsertOrReplaceInto(CAcquisitionSummaryTable::getTableName(), CAcquisitionSummaryTable::getTypeColumnName(), CAcquisitionSummaryTable::getDateColumnName(), CAcquisitionSummaryTable::getKeywordIdColumnName(), CAcquisitionSummaryTable::getAvgColumnName(), CAcquisitionSummaryTable::getMinColumnName(), CAcquisitionSummaryTable::getMaxColumnName()).
          Select(CQueryValue(curType.getAsString()).str(), CQueryValue(dayDate).str(), CQueryValue(keywordId).str(), "avg(cast(" + CAcquisitionTable::getValueColumnName() + " as real))", "min(cast(" + CAcquisitionTable::getValueColumnName() + " as real))", "max(cast(" + CAcquisitionTable::getValueColumnName() + " as real))").
          From(CAcquisitionTable::getTableName() + " as acq").
          Where("acq." + CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
          And("acq." + CAcquisitionTable::getDateColumnName(), CQUERY_OP_SUP_EQUAL, dayDate).
          And("acq." + CAcquisitionTable::getDateColumnName(), CQUERY_OP_INF_EQUAL, dayDateEnd);

         if (m_databaseRequester->queryStatement(q) <= 0)
            throw shared::exception::CEmptyResult("Fail to insert new summary day data");

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

