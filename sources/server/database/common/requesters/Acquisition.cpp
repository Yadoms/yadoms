#include "stdafx.h"
#include "Acquisition.h"
#include <shared/exception/NotImplemented.hpp>
#include <shared/exception/EmptyResult.hpp>
#include "database/common/DatabaseTables.h"
#include "database/common/Query.h"
#include "database/common/adapters/DatabaseAdapters.h"
#include "database/common/adapters/HighchartValueAdapter.hpp"
#include "database/common/adapters/SingleValueAdapter.hpp"
#include "database/common/adapters/MultipleValueAdapter.hpp"
#include <Poco/Timestamp.h>
#include <shared/currentTime/Provider.h>
#include "Keyword.h"

namespace database {  namespace common {  namespace requesters { 



   CAcquisition::CAcquisition(boost::shared_ptr<IDatabaseRequester> databaseRequester, boost::shared_ptr<CKeyword> keywordRequester)
      :m_keywordRequester(keywordRequester), m_databaseRequester(databaseRequester)
   {
   }

   CAcquisition::~CAcquisition()
   {
   }

   // IAcquisitionRequester implementation
   boost::shared_ptr<entities::CAcquisition> CAcquisition::saveData(const int keywordId, const std::string & data, boost::posix_time::ptime & dataTime)
   {
      if(m_keywordRequester->getKeyword(keywordId))
      {
         CQuery q = m_databaseRequester->newQuery();
         q.InsertInto(CAcquisitionTable::getTableName(), CAcquisitionTable::getDateColumnName(), CAcquisitionTable::getKeywordIdColumnName(), CAcquisitionTable::getValueColumnName()).
            Values(dataTime, keywordId, data);

         if(m_databaseRequester->queryStatement(q) <= 0)
            throw shared::exception::CEmptyResult("Fail to insert new data");

         return getAcquisitionByKeywordAndDate(keywordId, dataTime);
      }
      throw shared::exception::CEmptyResult("The keyword do not exists, cannot add data");
   }

   boost::shared_ptr<entities::CAcquisition> CAcquisition::incrementData(const int keywordId, const std::string & increment, boost::posix_time::ptime & dataTime)
   {
      boost::shared_ptr<entities::CKeyword> keywordEntity = m_keywordRequester->getKeyword(keywordId);

      if(!keywordEntity)
         throw shared::exception::CEmptyResult("The keyword do not exists, cannot increment data");

      if (keywordEntity->Type() != shared::plugin::yPluginApi::EKeywordDataType::kNumeric)
         throw shared::exception::CEmptyResult("The keyword is not numeric, cannot increment data");

      CQuery qLastKeywordValue = m_databaseRequester->newQuery();
      qLastKeywordValue.Select(qLastKeywordValue.castNumeric(CAcquisitionTable::getValueColumnName())).
         From(CAcquisitionTable::getTableName()).
         Where(CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
         OrderBy(CAcquisitionTable::getDateColumnName(), CQuery::kDesc).
         Limit(1);

      CQuery q = m_databaseRequester->newQuery();

      
      
      //insert first (if fails, update )
      q.InsertInto(CAcquisitionTable::getTableName(), CAcquisitionTable::getDateColumnName(), CAcquisitionTable::getKeywordIdColumnName(), CAcquisitionTable::getValueColumnName()).
      Values(dataTime, keywordId, q.math(q.coalesce(qLastKeywordValue, 0), CQUERY_OP_PLUS, increment));

      //q.InsertInto(CAcquisitionTable::getTableName(), CAcquisitionTable::getDateColumnName(), CAcquisitionTable::getKeywordIdColumnName(), CAcquisitionTable::getValueColumnName()).
      //Values(dataTime, keywordId, m_databaseRequester->queryFunc().coalesce(qLastKeywordValue, "0") + " + " + increment);
      
      if (m_databaseRequester->queryStatement(q, false) <= 0)
      {
        //update
         q.Clear().Update(CAcquisitionTable::getTableName())
         .Set(CAcquisitionTable::getValueColumnName(), q.math(q.coalesce(qLastKeywordValue, 0), CQUERY_OP_PLUS, increment)).
         Where(CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
         And(CAcquisitionTable::getDateColumnName(), CQUERY_OP_EQUAL, dataTime);
         
         if (m_databaseRequester->queryStatement(q) <= 0)
            throw shared::exception::CEmptyResult("Fail to insert new incremental data");

      }
      
      return getAcquisitionByKeywordAndDate(keywordId, dataTime);

   }

   void CAcquisition::removeKeywordData(const int keywordId)
   {
      if(m_keywordRequester->getKeyword(keywordId))
      {
         CQuery q = m_databaseRequester->newQuery();
         q.DeleteFrom(CAcquisitionTable::getTableName()).
            Where(CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId);

         m_databaseRequester->queryStatement(q);

         CQuery qSummary = m_databaseRequester->newQuery();
         qSummary.DeleteFrom(CAcquisitionSummaryTable::getTableName()).
            Where(CAcquisitionSummaryTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId);
         m_databaseRequester->queryStatement(qSummary);
      }
      else
      {
         throw shared::exception::CEmptyResult("The keyword do not exists, cannot delete data");
      }
   }

   boost::shared_ptr<entities::CAcquisition> CAcquisition::getAcquisitionByKeywordAndDate(const int keywordId, boost::posix_time::ptime time)
   {
      CQuery qSelect = m_databaseRequester->newQuery();
      qSelect.Select().
         From(CAcquisitionTable::getTableName()).
         Where(CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
         And(CAcquisitionTable::getDateColumnName(), CQUERY_OP_EQUAL, time);

      database::common::adapters::CAcquisitionAdapter adapter;
      m_databaseRequester->queryEntities(&adapter, qSelect);

      if (adapter.getResults().size() >= 1)
      {
         return adapter.getResults()[0];
      }
      std::string sEx = (boost::format("Cannot retrieve acquisition for KeywordId=%1% and date=%2%  in database") % keywordId % time).str();
      throw shared::exception::CEmptyResult(sEx);
   }


   boost::shared_ptr<entities::CAcquisition> CAcquisition::getKeywordLastData(const int keywordId)
   {
      CQuery qSelect = m_databaseRequester->newQuery();
      qSelect. Select().
               From(CAcquisitionTable::getTableName()).
               Where(CAcquisitionTable::getKeywordIdColumnName() , CQUERY_OP_EQUAL, keywordId).
               OrderBy(CAcquisitionTable::getDateColumnName(), CQuery::kDesc).
               Limit(1);

      adapters::CAcquisitionAdapter adapter;
      m_databaseRequester->queryEntities(&adapter, qSelect);

      if(adapter.getResults().size() >= 1)
      {
         return adapter.getResults()[0];
      }
      std::string sEx = (boost::format("Cannot retrieve any acquisition for the keyword id=%1% in database") % keywordId).str(); 
      throw shared::exception::CEmptyResult(sEx);
   }

   std::vector<boost::tuple<boost::posix_time::ptime, std::string>  > CAcquisition::getKeywordData(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
   {
      CQuery qSelect = m_databaseRequester->newQuery();
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

      database::common::adapters::CMultipleValueAdapter<boost::posix_time::ptime, std::string> mva;
      m_databaseRequester->queryEntities(&mva, qSelect);

      return mva.getResults();
   }


   std::vector<boost::shared_ptr<entities::CAcquisitionSummary> > CAcquisition::getKeywordDataByDay(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
   {
      return getKeywordSummaryDataByType(entities::EAcquisitionSummaryType::kDay, keywordId, timeFrom, timeTo);
   }



   std::vector<boost::shared_ptr<entities::CAcquisitionSummary> > CAcquisition::getKeywordDataByHour(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
   {
      return getKeywordSummaryDataByType(entities::EAcquisitionSummaryType::kHour, keywordId, timeFrom, timeTo);
   }




   std::vector<boost::shared_ptr<entities::CAcquisitionSummary> > CAcquisition::getKeywordSummaryDataByType(const entities::EAcquisitionSummaryType & type, int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
   {
      CQuery qSelect = m_databaseRequester->newQuery();
      qSelect.Select().
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

      if (type == entities::EAcquisitionSummaryType::kDay)
         qSelect.Limit(2000); //more than 5 years
      else
         qSelect.Limit(200); // more than 8 days


      adapters::CAcquisitionSummaryAdapter adapter;
      m_databaseRequester->queryEntities(&adapter, qSelect);

      return adapter.getResults();
   }





   std::string CAcquisition::getKeywordHighchartData(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
   {
      //this query is optimized to get only one field to read
      //the output data is a single column (without brackets):  ["dateiso",value]
      CQuery qSelect = m_databaseRequester->newQuery();
      qSelect.Select(qSelect.concatenate(qSelect.dateToIsoString(CAcquisitionTable::getDateColumnName()), "," , CAcquisitionTable::getValueColumnName())).
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
      adapters::CHighchartValueAdapter mva;

      m_databaseRequester->queryEntities(&mva, qSelect);

      return mva.getRawResults();
   }

   std::string CAcquisition::getKeywordHighchartDataByType(const entities::EAcquisitionSummaryType & type, int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
   {
      //this query is optimized to get only one field to read
      //the output data is a single column (without brackets):  ["dateiso",value]
      CQuery qSelect = m_databaseRequester->newQuery();
      qSelect.Select(qSelect.concatenate(qSelect.dateToIsoString(CAcquisitionSummaryTable::getDateColumnName()),  ",", CAcquisitionSummaryTable::getAvgColumnName())).
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
      adapters::CHighchartValueAdapter mva;

      m_databaseRequester->queryEntities(&mva, qSelect);

      return mva.getRawResults();

   }


   std::string CAcquisition::getKeywordHighchartDataByDay(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
   {
      return getKeywordHighchartDataByType(entities::EAcquisitionSummaryType::kDay, keywordId, timeFrom, timeTo);
   }

   std::string CAcquisition::getKeywordHighchartDataByHour(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
   {
      return getKeywordHighchartDataByType(entities::EAcquisitionSummaryType::kHour, keywordId, timeFrom, timeTo);
   }

   void CAcquisition::getKeywordsHavingDate(const boost::posix_time::ptime & timeFrom, const boost::posix_time::ptime & timeTo, std::vector<int> & results)
   {
      /*

      select distinct keywordId
      from Acquisition
      where date >= '20151105T100000'
      and date < '20151105T110000'

      */
      CQuery q = m_databaseRequester->newQuery();
      q.Select(q.distinct(CAcquisitionTable::getKeywordIdColumnName()))
         .From(CAcquisitionTable::getTableName())
         .Where(CAcquisitionTable::getDateColumnName(), CQUERY_OP_SUP_EQUAL, timeFrom)
         .And(CAcquisitionTable::getDateColumnName(), CQUERY_OP_INF, timeTo);

      database::common::adapters::CSingleValueAdapterWithContainer<int> intVectorAdapter(results);
      m_databaseRequester->queryEntities(&intVectorAdapter, q);
   }


   boost::shared_ptr<entities::CAcquisitionSummary> CAcquisition::saveSummaryData(const int keywordId, database::entities::EAcquisitionSummaryType curType, boost::posix_time::ptime & dataTime)
   {
      /* 
      INSERT OR REPLACE INTO AcquisitionSummary (type, date, keywordId, mean, min, max)
      SELECT "Hour", "startDate", 38, avg(cast(value as real)), min(cast(value as real)), max(cast(value as real))
      FROM Acquisition acq
      where acq.keywordId = 38
      and acq.date>= "startDate"
      and acq.date<= "endDate"
      
      */
      boost::shared_ptr<entities::CKeyword> keyword = m_keywordRequester->getKeyword(keywordId);

      if (keyword)
      {
         if (keyword->Type() == shared::plugin::yPluginApi::EKeywordDataType::kNumeric)
         {
            //just compute good dates
            //hourDate : is the start of the hour (current day) => minutes and seconds set to 0
            //hourDateEnd : is the end of the hour (current day) => minutes and seconds set to 59
            //dayDate : is the start of the day : current day, with hour, minte and second set to 0
            //dayDateEnd : is the end of the day : current day, with hour set to 23, minutes and seconds at 59
            boost::posix_time::ptime fromDate, toDate;
            tm pt_tm = boost::posix_time::to_tm(dataTime);
            if (curType == database::entities::EAcquisitionSummaryType::kHour)
            {
               fromDate = boost::posix_time::ptime(dataTime.date(), boost::posix_time::hours(pt_tm.tm_hour));
               toDate = boost::posix_time::ptime(dataTime.date(), boost::posix_time::hours(pt_tm.tm_hour) + boost::posix_time::minutes(59) + boost::posix_time::seconds(59));
            }
            else if (curType == database::entities::EAcquisitionSummaryType::kDay)
            {
               fromDate = boost::posix_time::ptime(dataTime.date());
               toDate = boost::posix_time::ptime(dataTime.date(), boost::posix_time::hours(23) + boost::posix_time::minutes(59) + boost::posix_time::seconds(59));
            }

            //process the request
            CQuery q = m_databaseRequester->newQuery();
            q.InsertInto(CAcquisitionSummaryTable::getTableName(), CAcquisitionSummaryTable::getTypeColumnName(), CAcquisitionSummaryTable::getDateColumnName(), CAcquisitionSummaryTable::getKeywordIdColumnName(), CAcquisitionSummaryTable::getAvgColumnName(), CAcquisitionSummaryTable::getMinColumnName(), CAcquisitionSummaryTable::getMaxColumnName()).
               Select(curType, fromDate, keywordId, q.averageWithCast(CAcquisitionTable::getValueColumnName()), q.minWithCast(CAcquisitionTable::getValueColumnName()), q.maxWithCast(CAcquisitionTable::getValueColumnName())).
               From(q.as(CAcquisitionTable::getTableName(), "acq")).
               Where(q.fromSubquery("acq", CAcquisitionTable::getKeywordIdColumnName()), CQUERY_OP_EQUAL, keywordId).
               And(q.fromSubquery("acq", CAcquisitionTable::getDateColumnName()), CQUERY_OP_SUP_EQUAL, fromDate).
               And(q.fromSubquery("acq", CAcquisitionTable::getDateColumnName()), CQUERY_OP_INF_EQUAL, toDate);
            
            if (m_databaseRequester->queryStatement(q, false) <= 0)
            {
               //the insertion fails, try to update
               CQuery compute = m_databaseRequester->newQuery();
               
               compute.Select(compute.as(compute.averageWithCast(CAcquisitionTable::getValueColumnName()), "avg"), 
                              compute.as(compute.minWithCast(CAcquisitionTable::getValueColumnName()), "min"), 
                              compute.as(compute.maxWithCast(CAcquisitionTable::getValueColumnName()), "max")).
                  From(CAcquisitionTable::getTableName()).
                  Where(CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
                  And(CAcquisitionTable::getDateColumnName(), CQUERY_OP_SUP_EQUAL, fromDate).
                  And(CAcquisitionTable::getDateColumnName(), CQUERY_OP_INF_EQUAL, toDate);
               
               
               q.Clear().With("acq", compute).
                 Update(CAcquisitionSummaryTable::getTableName())
               .Set(CAcquisitionSummaryTable::getAvgColumnName(),  q.fromSubquery("acq", "avg"),
                    CAcquisitionSummaryTable::getMinColumnName(), q.fromSubquery("acq", "min"),
                    CAcquisitionSummaryTable::getMaxColumnName(), q.fromSubquery("acq", "max"))
               .Where(CAcquisitionSummaryTable::getTypeColumnName(), CQUERY_OP_EQUAL, curType.toString())
               .And(CAcquisitionSummaryTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL,keywordId)
               .And(CAcquisitionSummaryTable::getDateColumnName(), CQUERY_OP_EQUAL,fromDate);
               
               if (m_databaseRequester->queryStatement(q) <= 0)
                  throw shared::exception::CEmptyResult("Fail to insert new summary " + curType.toString() + " data");
               

            }

            //get the result
            std::vector< boost::shared_ptr<entities::CAcquisitionSummary> > all;
            if (curType == database::entities::EAcquisitionSummaryType::kDay)
               all = getKeywordDataByDay(keywordId, fromDate, toDate);
            else 
               all = getKeywordDataByHour(keywordId, fromDate, toDate);
            if (!all.empty())
               return all[0];
         }
         //keyword is not numeric, no data to avg, min and max !
         return boost::shared_ptr<entities::CAcquisitionSummary>();
      }
      throw shared::exception::CEmptyResult("The keyword do not exists, cannot add summary data");
   }

   bool CAcquisition::summaryDataExists(const int keywordId, entities::EAcquisitionSummaryType curType, boost::posix_time::ptime & dataTime)
   {
      //determine the real date of summary data 
      boost::posix_time::ptime fromDate;
      tm pt_tm = boost::posix_time::to_tm(dataTime);
      if (curType == database::entities::EAcquisitionSummaryType::kHour)
      {
         fromDate = boost::posix_time::ptime(dataTime.date(), boost::posix_time::hours(pt_tm.tm_hour));
      }
      else if (curType == database::entities::EAcquisitionSummaryType::kDay)
      {
         fromDate = boost::posix_time::ptime(dataTime.date());
      }

      CQuery checkq = m_databaseRequester->newQuery();
      checkq.SelectCount().
         From(CAcquisitionSummaryTable::getTableName()).
         Where(CAcquisitionSummaryTable::getTypeColumnName(), CQUERY_OP_EQUAL, curType).
         And(CAcquisitionSummaryTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
         And(CAcquisitionSummaryTable::getDateColumnName(), CQUERY_OP_EQUAL, fromDate);

      return (m_databaseRequester->queryCount(checkq) > 0);
   }

   int CAcquisition::purgeAcquisitions(boost::posix_time::ptime purgeDate)
   {
      CQuery q = m_databaseRequester->newQuery();
      q.DeleteFrom(CAcquisitionTable::getTableName()).
         Where(CAcquisitionTable::getDateColumnName(), CQUERY_OP_INF, purgeDate);

      int count = m_databaseRequester->queryStatement(q);
      if (count < 0)
         throw shared::exception::CException("Fail to purge database");
      return count;
   }
   // [END] IAcquisitionRequester implementation


} //namespace requesters
} //namespace common
} //namespace database 

