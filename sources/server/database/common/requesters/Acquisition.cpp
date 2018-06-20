#include "stdafx.h"
#include "Acquisition.h"
#include <shared/exception/EmptyResult.hpp>
#include "database/common/DatabaseTables.h"
#include "database/common/Query.h"
#include "database/common/adapters/DatabaseAdapters.h"
#include "database/common/adapters/HugeDataVectorAdapter.hpp"
#include "database/common/adapters/SingleValueAdapter.hpp"
#include "database/common/adapters/MultipleValueAdapter.hpp"
#include "database/DatabaseException.hpp"
#include "Keyword.h"

namespace database
{
   namespace common
   {
      namespace requesters
      {
         CAcquisition::CAcquisition(boost::shared_ptr<IDatabaseRequester> databaseRequester, boost::shared_ptr<CKeyword> keywordRequester)
            : m_keywordRequester(keywordRequester), m_databaseRequester(databaseRequester)
         {
         }

         CAcquisition::~CAcquisition()
         {
         }

         // IAcquisitionRequester implementation
         boost::shared_ptr<entities::CAcquisition> CAcquisition::saveData(const int keywordId, const std::string& data, boost::posix_time::ptime& dataTime)
         {
            auto keywordEntity = m_keywordRequester->getKeyword(keywordId);
            if (keywordEntity)
            {
               if (!keywordEntity->Blacklist())
               {
                  auto q = m_databaseRequester->newQuery();
                  q->InsertInto(CAcquisitionTable::getTableName(), CAcquisitionTable::getDateColumnName(), CAcquisitionTable::getKeywordIdColumnName(), CAcquisitionTable::getValueColumnName()).
                     Values(dataTime, keywordId, data);

                  try
                  {
                     if (m_databaseRequester->queryStatement(*q) <= 0)
                        throw shared::exception::CEmptyResult("Fail to insert new data");
                  }
                  catch (CDatabaseException& e)
                  {
                     if (e.returnCode() == CDatabaseException::kConstraintViolation)
                     {
                        // Maybe 2 acquisitions were recorded at same time for same keyword. In this case, we prefer to keep last value
                        q->Clear().Update(CAcquisitionTable::getTableName())
                           .Set(CAcquisitionTable::getValueColumnName(), data)
                           .Where(CAcquisitionTable::getDateColumnName(), CQUERY_OP_EQUAL, dataTime)
                           .And(CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId);
                        m_databaseRequester->queryStatement(*q);
                     }
                     else
                     {
                        throw;
                     }
                  }

                  // Update also last value in keyword table
                  m_keywordRequester->updateLastValue(keywordId,
                                                      dataTime,
                                                      data);

                  return getAcquisitionByKeywordAndDate(keywordId, dataTime);
               }

               //blacklisted keyword
               return boost::shared_ptr<entities::CAcquisition>();
            }
            throw shared::exception::CEmptyResult("The keyword do not exists, cannot add data");
         }

         boost::shared_ptr<entities::CAcquisition> CAcquisition::incrementData(const int keywordId, const std::string& increment, boost::posix_time::ptime& dataTime)
         {
            auto keywordEntity = m_keywordRequester->getKeyword(keywordId);

            if (!keywordEntity)
               throw shared::exception::CEmptyResult("The keyword do not exists, cannot increment data");

            if (keywordEntity->Type() != shared::plugin::yPluginApi::EKeywordDataType::kNumeric)
               throw shared::exception::CEmptyResult("The keyword is not numeric, cannot increment data");

            if (keywordEntity->Blacklist())
               return boost::shared_ptr<entities::CAcquisition>(); //return null instead of exception for performances

            auto qLastKeywordValue = m_databaseRequester->newQuery();
            qLastKeywordValue->Select(qLastKeywordValue->castNumeric(CAcquisitionTable::getValueColumnName())).
               From(CAcquisitionTable::getTableName()).
               Where(CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
               OrderBy(CAcquisitionTable::getDateColumnName(), CQuery::kDesc).
               Limit(1);

            auto q = m_databaseRequester->newQuery();


            //insert first (if fails, update )
            q->InsertInto(CAcquisitionTable::getTableName(), CAcquisitionTable::getDateColumnName(), CAcquisitionTable::getKeywordIdColumnName(), CAcquisitionTable::getValueColumnName()).
               Values(dataTime, keywordId, q->math(q->coalesce(*qLastKeywordValue, 0), CQUERY_OP_PLUS, increment));

            if (m_databaseRequester->queryStatement(*q, false) <= 0)
            {
               //update
               q->Clear().Update(CAcquisitionTable::getTableName())
                  .Set(CAcquisitionTable::getValueColumnName(), q->math(q->coalesce(*qLastKeywordValue, 0), CQUERY_OP_PLUS, increment)).
                  Where(CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
                  And(CAcquisitionTable::getDateColumnName(), CQUERY_OP_EQUAL, dataTime);

               if (m_databaseRequester->queryStatement(*q) <= 0)
                  throw shared::exception::CEmptyResult("Fail to insert new incremental data");
            }

            auto newData = getAcquisitionByKeywordAndDate(keywordId, dataTime);

            // Update also last value in keyword table
            m_keywordRequester->updateLastValue(keywordId,
                                                dataTime,
                                                newData->Value());

            return newData;
         }

         void CAcquisition::removeKeywordData(const int keywordId)
         {
            if (m_keywordRequester->getKeyword(keywordId))
            {
               auto q = m_databaseRequester->newQuery();
               q->DeleteFrom(CAcquisitionTable::getTableName()).
                  Where(CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId);

               m_databaseRequester->queryStatement(*q);

               auto qSummary = m_databaseRequester->newQuery();
               qSummary->DeleteFrom(CAcquisitionSummaryTable::getTableName()).
                  Where(CAcquisitionSummaryTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId);
               m_databaseRequester->queryStatement(*qSummary);
            }
            else
            {
               throw shared::exception::CEmptyResult("The keyword do not exists, cannot delete data");
            }
         }

         boost::shared_ptr<entities::CAcquisition> CAcquisition::getAcquisitionByKeywordAndDate(const int keywordId, boost::posix_time::ptime time)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
               From(CAcquisitionTable::getTableName()).
               Where(CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
               And(CAcquisitionTable::getDateColumnName(), CQUERY_OP_EQUAL, time);

            adapters::CAcquisitionAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);

            if (adapter.getResults().size() >= 1)
            {
               return adapter.getResults()[0];
            }

            throw shared::exception::CEmptyResult((boost::format("Cannot retrieve acquisition for KeywordId=%1% and date=%2%  in database") % keywordId % time).str());
         }

         std::vector<boost::tuple<boost::posix_time::ptime, std::string>> CAcquisition::getKeywordData(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select(CAcquisitionTable::getDateColumnName(), CAcquisitionTable::getValueColumnName()).
               From(CAcquisitionTable::getTableName()).
               Where(CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId);

            if (!timeFrom.is_not_a_date_time())
            {
               qSelect->And(CAcquisitionTable::getDateColumnName(), CQUERY_OP_SUP_EQUAL, timeFrom);
               if (!timeTo.is_not_a_date_time())
               {
                  qSelect->And(CAcquisitionTable::getDateColumnName(), CQUERY_OP_INF_EQUAL, timeTo);
               }
            }

            qSelect->OrderBy(CAcquisitionTable::getDateColumnName());

            adapters::CMultipleValueAdapter<boost::posix_time::ptime, std::string> mva;
            m_databaseRequester->queryEntities(&mva, *qSelect);

            return mva.getResults();
         }

         std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary>> CAcquisition::getKeywordDataByHour(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
         {
            return getKeywordSummaryDataByType(entities::EAcquisitionSummaryType::kHour, keywordId, timeFrom, timeTo);
         }

         std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary>> CAcquisition::getKeywordDataByDay(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
         {
            return getKeywordSummaryDataByType(entities::EAcquisitionSummaryType::kDay, keywordId, timeFrom, timeTo);
         }

         std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary>> CAcquisition::getKeywordDataByMonth(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
         {
            return getKeywordSummaryDataByType(entities::EAcquisitionSummaryType::kMonth, keywordId, timeFrom, timeTo);
         }

         std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary>> CAcquisition::getKeywordDataByYear(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
         {
            return getKeywordSummaryDataByType(entities::EAcquisitionSummaryType::kYear, keywordId, timeFrom, timeTo);
         }

         std::vector< boost::shared_ptr<database::entities::CAcquisitionSummary>> CAcquisition::getKeywordSummaryDataByType(const entities::EAcquisitionSummaryType& type, int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo) const
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
               From(CAcquisitionSummaryTable::getTableName()).
               Where(CAcquisitionSummaryTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
               And(CAcquisitionSummaryTable::getTypeColumnName(), CQUERY_OP_EQUAL, type);

            if (!timeFrom.is_not_a_date_time())
            {
               qSelect->And(CAcquisitionSummaryTable::getDateColumnName(), CQUERY_OP_SUP_EQUAL, timeFrom);
               if (!timeTo.is_not_a_date_time())
               {
                  qSelect->And(CAcquisitionSummaryTable::getDateColumnName(), CQUERY_OP_INF_EQUAL, timeTo);
               }
            }

            qSelect->OrderBy(CAcquisitionSummaryTable::getDateColumnName());
            adapters::CAcquisitionSummaryAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);

            return adapter.getResults();
         }

         std::string CAcquisition::getHugeVectorKeywordDataByHour(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
         {
            return getHugeVectorKeywordSummaryDataByType(entities::EAcquisitionSummaryType::kHour, keywordId, timeFrom, timeTo);
         }

         std::string CAcquisition::getHugeVectorKeywordDataByDay(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
         {
            return getHugeVectorKeywordSummaryDataByType(entities::EAcquisitionSummaryType::kDay, keywordId, timeFrom, timeTo);
         }

         std::string CAcquisition::getHugeVectorKeywordDataByMonth(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
         {
            return getHugeVectorKeywordSummaryDataByType(entities::EAcquisitionSummaryType::kMonth, keywordId, timeFrom, timeTo);
         }

         std::string CAcquisition::getHugeVectorKeywordDataByYear(int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo)
         {
            return getHugeVectorKeywordSummaryDataByType(entities::EAcquisitionSummaryType::kYear, keywordId, timeFrom, timeTo);
         }

         std::string CAcquisition::getHugeVectorKeywordSummaryDataByType(const entities::EAcquisitionSummaryType& type, int keywordId, boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo) const
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
               From(CAcquisitionSummaryTable::getTableName()).
               Where(CAcquisitionSummaryTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
               And(CAcquisitionSummaryTable::getTypeColumnName(), CQUERY_OP_EQUAL, type);

            if (!timeFrom.is_not_a_date_time())
            {
               qSelect->And(CAcquisitionSummaryTable::getDateColumnName(), CQUERY_OP_SUP_EQUAL, timeFrom);
               if (!timeTo.is_not_a_date_time())
               {
                  qSelect->And(CAcquisitionSummaryTable::getDateColumnName(), CQUERY_OP_INF_EQUAL, timeTo);
               }
            }

            qSelect->OrderBy(CAcquisitionSummaryTable::getDateColumnName());
            adapters::CHugeDataVectorAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);

            return adapter.getRawResults();
         }

         void CAcquisition::getKeywordsHavingDate(const boost::posix_time::ptime& timeFrom, const boost::posix_time::ptime& timeTo, std::vector<int>& results)
         {
            /*
      
            select distinct keywordId
            from Acquisition
            where date >= '20151105T100000'
            and date < '20151105T110000'
      
            */
            auto q = m_databaseRequester->newQuery();
            q->Select(q->distinct(CAcquisitionTable::getKeywordIdColumnName()))
               .From(CAcquisitionTable::getTableName())
               .Where(CAcquisitionTable::getDateColumnName(), CQUERY_OP_SUP_EQUAL, timeFrom)
               .And(CAcquisitionTable::getDateColumnName(), CQUERY_OP_INF, timeTo);

            adapters::CSingleValueAdapterWithContainer<int> intVectorAdapter(results);
            m_databaseRequester->queryEntities(&intVectorAdapter, *q);
         }


         boost::shared_ptr<entities::CAcquisitionSummary> CAcquisition::saveSummaryData(const int keywordId, entities::EAcquisitionSummaryType curType, boost::posix_time::ptime& dataTime)
         {
            /* 
            INSERT OR REPLACE INTO AcquisitionSummary (type, date, keywordId, mean, min, max)
            SELECT "Hour", "startDate", 38, avg(cast(value as real)), min(cast(value as real)), max(cast(value as real))
            FROM Acquisition acq
            where acq.keywordId = 38
            and acq.date>= "startDate"
            and acq.date<= "endDate"
            
            */
            auto keyword = m_keywordRequester->getKeyword(keywordId);

            if (keyword)
            {
               if (keyword->Type() == shared::plugin::yPluginApi::EKeywordDataType::kNumeric)
               {
                  //just compute good dates
                  //hourDate : is the start of the hour (current day) => minutes and seconds set to 0
                  //hourDateEnd : is the end of the hour (current day) => minutes and seconds set to 59
                  //dayDate : is the start of the day : current day, with hour, minte and second set to 0
                  //dayDateEnd : is the end of the day : current day, with hour set to 23, minutes and seconds at 59


                  //to optimize queries, just do
                  //    update line
                  //       if >1 line is updated, nothing to do
                  //       else (0) no line updated, then insert new line
                  //
                  // so in good cases : only 1 query is realized
                  // so in bad cases : only 2 queries are realized


                  boost::posix_time::ptime fromDate, toDate;
                  const auto pt_tm = boost::posix_time::to_tm(dataTime);

                  //Hour summary data are commputed from raw acquisitions
                  //Other ones, are taken from summarydata (really simplify calculous, and queries)

                  if (curType == entities::EAcquisitionSummaryType::kHour)
                  {
                     fromDate = boost::posix_time::ptime(dataTime.date(), boost::posix_time::hours(pt_tm.tm_hour));
                     toDate = boost::posix_time::ptime(dataTime.date(), boost::posix_time::hours(pt_tm.tm_hour) + boost::posix_time::minutes(59) + boost::posix_time::seconds(59));

                     if (m_databaseRequester->supportInsertOrUpdateStatement())
                     {
                        //process the request
                        auto q = m_databaseRequester->newQuery();
                        q->InsertOrReplaceInto(CAcquisitionSummaryTable::getTableName(), CAcquisitionSummaryTable::getTypeColumnName(), CAcquisitionSummaryTable::getDateColumnName(), CAcquisitionSummaryTable::getKeywordIdColumnName(), CAcquisitionSummaryTable::getAvgColumnName(), CAcquisitionSummaryTable::getMinColumnName(), CAcquisitionSummaryTable::getMaxColumnName()).
                           Select(curType, fromDate, keywordId, q->averageWithCast(q->fromSubquery("acq", CAcquisitionTable::getValueColumnName())),
                                  q->minWithCast(q->fromSubquery("acq", CAcquisitionTable::getValueColumnName())),
                                  q->maxWithCast(q->fromSubquery("acq", CAcquisitionTable::getValueColumnName()))).
                           From(q->as(CAcquisitionTable::getTableName(), "acq")).
                           Where(q->fromSubquery("acq", CAcquisitionTable::getKeywordIdColumnName()), CQUERY_OP_EQUAL, keywordId).
                           And(q->fromSubquery("acq", CAcquisitionTable::getDateColumnName()), CQUERY_OP_SUP_EQUAL, fromDate).
                           And(q->fromSubquery("acq", CAcquisitionTable::getDateColumnName()), CQUERY_OP_INF_EQUAL, toDate);
                        if (m_databaseRequester->queryStatement(*q) <= 0)
                           throw shared::exception::CEmptyResult("Fail to insert summary value (insert or update) " + curType.toString() + " data");
                     }
                     else
                     {
                        //update
                        auto compute = m_databaseRequester->newQuery();

                        compute->Select(compute->as(compute->averageWithCast(CAcquisitionTable::getValueColumnName()), "avg"),
                                        compute->as(compute->minWithCast(CAcquisitionTable::getValueColumnName()), "min"),
                                        compute->as(compute->maxWithCast(CAcquisitionTable::getValueColumnName()), "max")).
                           From(CAcquisitionTable::getTableName()).
                           Where(CAcquisitionTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
                           And(CAcquisitionTable::getDateColumnName(), CQUERY_OP_SUP_EQUAL, fromDate).
                           And(CAcquisitionTable::getDateColumnName(), CQUERY_OP_INF_EQUAL, toDate);

                        auto q = m_databaseRequester->newQuery();


                        q->With("acq", *compute).
                           Update(CAcquisitionSummaryTable::getTableName())
                           .Set(CAcquisitionSummaryTable::getAvgColumnName(), q->fromSubquery("acq", "avg"),
                                CAcquisitionSummaryTable::getMinColumnName(), q->fromSubquery("acq", "min"),
                                CAcquisitionSummaryTable::getMaxColumnName(), q->fromSubquery("acq", "max"))
                           .FromWith("acq")
                           .Where(CAcquisitionSummaryTable::getTypeColumnName(), CQUERY_OP_EQUAL, curType.toString())
                           .And(CAcquisitionSummaryTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId)
                           .And(CAcquisitionSummaryTable::getDateColumnName(), CQUERY_OP_EQUAL, fromDate);

                        if (m_databaseRequester->queryStatement(*q) <= 0)
                        {
                           //fail to update, then insert
                           //insert
                           q->Clear();
                           q->InsertInto(CAcquisitionSummaryTable::getTableName(), CAcquisitionSummaryTable::getTypeColumnName(), CAcquisitionSummaryTable::getDateColumnName(), CAcquisitionSummaryTable::getKeywordIdColumnName(), CAcquisitionSummaryTable::getAvgColumnName(), CAcquisitionSummaryTable::getMinColumnName(), CAcquisitionSummaryTable::getMaxColumnName()).
                              Select(curType, fromDate, keywordId, q->averageWithCast(q->fromSubquery("acq", CAcquisitionTable::getValueColumnName())),
                                     q->minWithCast(q->fromSubquery("acq", CAcquisitionTable::getValueColumnName())),
                                     q->maxWithCast(q->fromSubquery("acq", CAcquisitionTable::getValueColumnName()))).
                              From(q->as(CAcquisitionTable::getTableName(), "acq")).
                              Where(q->fromSubquery("acq", CAcquisitionTable::getKeywordIdColumnName()), CQUERY_OP_EQUAL, keywordId).
                              And(q->fromSubquery("acq", CAcquisitionTable::getDateColumnName()), CQUERY_OP_SUP_EQUAL, fromDate).
                              And(q->fromSubquery("acq", CAcquisitionTable::getDateColumnName()), CQUERY_OP_INF_EQUAL, toDate);

                           if (m_databaseRequester->queryStatement(*q) <= 0)
                              throw shared::exception::CEmptyResult("Fail to insert summary value " + curType.toString() + " data");
                        }
                     }
                  }
                  else
                  {
                     entities::EAcquisitionSummaryType toQuery;

                     //all types but Hour
                     switch (curType)
                     {
                     case entities::EAcquisitionSummaryType::kDayValue:
                        fromDate = boost::posix_time::ptime(dataTime.date());
                        toDate = boost::posix_time::ptime(dataTime.date(),
                                                          boost::posix_time::hours(23) + boost::posix_time::minutes(59) + boost::posix_time::seconds(59));
                        toQuery = entities::EAcquisitionSummaryType::kHour;
                        break;

                     case entities::EAcquisitionSummaryType::kMonthValue:
                        fromDate = boost::posix_time::ptime(boost::gregorian::date(dataTime.date().year(),
                                                                                   dataTime.date().month(),
                                                                                   1));
                        toDate = boost::posix_time::ptime(boost::gregorian::date(dataTime.date().year(),
                                                                                 dataTime.date().month(),
                                                                                 boost::gregorian::gregorian_calendar::end_of_month_day(dataTime.date().year(),
                                                                                                                                        dataTime.date().month())),
                                                          boost::posix_time::hours(23) + boost::posix_time::minutes(59) + boost::posix_time::seconds(59));
                        toQuery = entities::EAcquisitionSummaryType::kDay;
                        break;

                     case entities::EAcquisitionSummaryType::kYearValue:
                        fromDate = boost::posix_time::ptime(boost::gregorian::date(dataTime.date().year(),
                                                                                   1,
                                                                                   1));
                        toDate = boost::posix_time::ptime(boost::gregorian::date(dataTime.date().year(),
                                                                                 12,
                                                                                 boost::gregorian::gregorian_calendar::end_of_month_day(dataTime.date().year(), 12)),
                                                          boost::posix_time::hours(23) + boost::posix_time::minutes(59) + boost::posix_time::seconds(59));
                        toQuery = entities::EAcquisitionSummaryType::kMonth;
                        break;
                  default: 
                     break;
                     }


                     if (m_databaseRequester->supportInsertOrUpdateStatement())
                     {
                        auto q = m_databaseRequester->newQuery();
                        q->InsertOrReplaceInto(CAcquisitionSummaryTable::getTableName(), CAcquisitionSummaryTable::getTypeColumnName(), CAcquisitionSummaryTable::getDateColumnName(), CAcquisitionSummaryTable::getKeywordIdColumnName(), CAcquisitionSummaryTable::getAvgColumnName(), CAcquisitionSummaryTable::getMinColumnName(), CAcquisitionSummaryTable::getMaxColumnName()).
                           Select(curType, fromDate, keywordId, q->average(q->fromSubquery("acq", CAcquisitionSummaryTable::getAvgColumnName())),
                                  q->min(q->fromSubquery("acq", CAcquisitionSummaryTable::getMinColumnName())),
                                  q->max(q->fromSubquery("acq", CAcquisitionSummaryTable::getMaxColumnName()))).
                           From(q->as(CAcquisitionSummaryTable::getTableName(), "acq")).
                           Where(q->fromSubquery("acq", CAcquisitionSummaryTable::getKeywordIdColumnName()), CQUERY_OP_EQUAL, keywordId).
                           And(q->fromSubquery("acq", CAcquisitionSummaryTable::getTypeColumnName()), CQUERY_OP_EQUAL, toQuery.toString()).
                           And(q->fromSubquery("acq", CAcquisitionSummaryTable::getDateColumnName()), CQUERY_OP_SUP_EQUAL, fromDate).
                           And(q->fromSubquery("acq", CAcquisitionSummaryTable::getDateColumnName()), CQUERY_OP_INF_EQUAL, toDate);

                        if (m_databaseRequester->queryStatement(*q) <= 0)
                           throw shared::exception::CEmptyResult("Fail to insert summary value (insert or update) " + curType.toString() + " data");
                     }
                     else
                     {
                        //update
                        auto compute = m_databaseRequester->newQuery();

                        compute->Select(compute->as(compute->average(CAcquisitionSummaryTable::getAvgColumnName()), "avg"),
                                        compute->as(compute->min(CAcquisitionSummaryTable::getMinColumnName()), "min"),
                                        compute->as(compute->max(CAcquisitionSummaryTable::getMaxColumnName()), "max")).
                           From(CAcquisitionSummaryTable::getTableName()).
                           Where(CAcquisitionSummaryTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
                           And(CAcquisitionSummaryTable::getTypeColumnName(), CQUERY_OP_EQUAL, toQuery.toString()).
                           And(CAcquisitionSummaryTable::getDateColumnName(), CQUERY_OP_SUP_EQUAL, fromDate).
                           And(CAcquisitionSummaryTable::getDateColumnName(), CQUERY_OP_INF_EQUAL, toDate);

                        auto q = m_databaseRequester->newQuery();

                        q->With("acq", *compute).
                           Update(CAcquisitionSummaryTable::getTableName())
                           .Set(CAcquisitionSummaryTable::getAvgColumnName(), q->fromSubquery("acq", "avg"),
                                CAcquisitionSummaryTable::getMinColumnName(), q->fromSubquery("acq", "min"),
                                CAcquisitionSummaryTable::getMaxColumnName(), q->fromSubquery("acq", "max"))
                           .FromWith("acq")
                           .Where(CAcquisitionSummaryTable::getTypeColumnName(), CQUERY_OP_EQUAL, curType.toString())
                           .And(CAcquisitionSummaryTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId)
                           .And(CAcquisitionSummaryTable::getDateColumnName(), CQUERY_OP_EQUAL, fromDate);

                        if (m_databaseRequester->queryStatement(*q) <= 0)
                        {
                           //fail to update, then insert
                           //insert
                           q->Clear();
                           q->InsertInto(CAcquisitionSummaryTable::getTableName(), CAcquisitionSummaryTable::getTypeColumnName(), CAcquisitionSummaryTable::getDateColumnName(), CAcquisitionSummaryTable::getKeywordIdColumnName(), CAcquisitionSummaryTable::getAvgColumnName(), CAcquisitionSummaryTable::getMinColumnName(), CAcquisitionSummaryTable::getMaxColumnName()).
                              Select(curType, fromDate, keywordId, q->average(q->fromSubquery("acq", CAcquisitionSummaryTable::getAvgColumnName())),
                                     q->min(q->fromSubquery("acq", CAcquisitionSummaryTable::getMinColumnName())),
                                     q->max(q->fromSubquery("acq", CAcquisitionSummaryTable::getMaxColumnName()))).
                              From(q->as(CAcquisitionSummaryTable::getTableName(), "acq")).
                              Where(q->fromSubquery("acq", CAcquisitionSummaryTable::getKeywordIdColumnName()), CQUERY_OP_EQUAL, keywordId).
                              And(q->fromSubquery("acq", CAcquisitionSummaryTable::getTypeColumnName()), CQUERY_OP_EQUAL, toQuery.toString()).
                              And(q->fromSubquery("acq", CAcquisitionSummaryTable::getDateColumnName()), CQUERY_OP_SUP_EQUAL, fromDate).
                              And(q->fromSubquery("acq", CAcquisitionSummaryTable::getDateColumnName()), CQUERY_OP_INF_EQUAL, toDate);

                           if (m_databaseRequester->queryStatement(*q) <= 0)
                              throw shared::exception::CEmptyResult("Fail to insert summary value " + curType.toString() + " data");
                        }
                     }
                  }

                  //get the result
                  std::vector<boost::shared_ptr<entities::CAcquisitionSummary>> all;
                  switch (curType)
                  {
                  case entities::EAcquisitionSummaryType::kHourValue:
                     all = getKeywordDataByHour(keywordId, fromDate, toDate);
                     break;

                  case entities::EAcquisitionSummaryType::kDayValue:
                     all = getKeywordDataByDay(keywordId, fromDate, toDate);
                     break;

                  case entities::EAcquisitionSummaryType::kMonthValue:
                     all = getKeywordDataByMonth(keywordId, fromDate, toDate);
                     break;

                  case entities::EAcquisitionSummaryType::kYearValue:
                     all = getKeywordDataByYear(keywordId, fromDate, toDate);
                     break;
                  default: 
                     throw shared::exception::CEmptyResult("Invalid summary type " + curType.toString());
                  }
                  if (!all.empty())
                     return all[0];
               }
               //keyword is not numeric, no data to avg, min and max !
               return boost::shared_ptr<entities::CAcquisitionSummary>();
            }
            throw shared::exception::CEmptyResult("The keyword do not exists, cannot add summary data");
         }

         bool CAcquisition::summaryDataExists(const int keywordId, entities::EAcquisitionSummaryType curType, boost::posix_time::ptime& dataTime)
         {
            //determine the real date of summary data 
            boost::posix_time::ptime fromDate;
            const auto pt_tm = boost::posix_time::to_tm(dataTime);
            if (curType == entities::EAcquisitionSummaryType::kHour)
            {
               fromDate = boost::posix_time::ptime(dataTime.date(), boost::posix_time::hours(pt_tm.tm_hour));
            }
            else if (curType == entities::EAcquisitionSummaryType::kDay)
            {
               fromDate = boost::posix_time::ptime(dataTime.date());
            }

            auto checkq = m_databaseRequester->newQuery();
            checkq->SelectCount().
               From(CAcquisitionSummaryTable::getTableName()).
               Where(CAcquisitionSummaryTable::getTypeColumnName(), CQUERY_OP_EQUAL, curType).
               And(CAcquisitionSummaryTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, keywordId).
               And(CAcquisitionSummaryTable::getDateColumnName(), CQUERY_OP_EQUAL, fromDate);

            return (m_databaseRequester->queryCount(*checkq) > 0);
         }

         int CAcquisition::purgeAcquisitions(boost::posix_time::ptime purgeDate)
         {
            auto q = m_databaseRequester->newQuery();
            q->DeleteFrom(CAcquisitionTable::getTableName()).
               Where(CAcquisitionTable::getDateColumnName(), CQUERY_OP_INF, purgeDate);

            const auto count = m_databaseRequester->queryStatement(*q);
            if (count < 0)
               throw shared::exception::CException("Fail to purge database");
            return count;
         }

         // [END] IAcquisitionRequester implementation
      } //namespace requesters
   } //namespace common
} //namespace database 


