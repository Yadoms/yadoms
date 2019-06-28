#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/enumeration/EnumHelpers.hpp"
#include "../../../../sources/server/database/entities/Entities.h"
#include "../../../../sources/server/database/common/Query.h"
#include "../../../../sources/server/database/pgsql/PgsqlQuery.h"
#include "../../../../sources/server/database/sqlite/SQLiteQuery.h"
#include "../../../../sources/server/database/common/DatabaseTables.h"



BOOST_AUTO_TEST_SUITE(TestQuery)

bool both_are_spaces(const char lhs, const char rhs) { return (lhs == rhs) && (lhs == ' '); }


std::string beautifyQuery(database::common::CQuery & q)
{
   //trim spaces
   auto trimm = boost::trim_copy(q.str());
   
   //replace any double space by only one
   const auto new_end = std::unique(trimm.begin(), trimm.end(), both_are_spaces);
   trimm.erase(new_end, trimm.end());

   return trimm;
}


BOOST_AUTO_TEST_CASE(Select)
{
   database::pgsql::CPgsqlQuery test1;
   test1.Select().From(database::common::CPluginTable::getTableName());
   BOOST_CHECK_EQUAL(beautifyQuery(test1), (boost::format("SELECT * FROM %1%") % database::common::CPluginTable::getTableName().GetName()).str());
      
   database::pgsql::CPgsqlQuery test2;
   test2.Select(database::common::CPluginTable::getIdColumnName()).From(database::common::CPluginTable::getTableName());
   BOOST_CHECK_EQUAL(beautifyQuery(test2), (boost::format("SELECT %1% FROM %2%") % database::common::CPluginTable::getIdColumnName().GetName() % database::common::CPluginTable::getTableName().GetName()).str());

   database::pgsql::CPgsqlQuery test3;
   test3.Select(database::common::CPluginTable::getIdColumnName(), database::common::CPluginTable::getDisplayNameColumnName()).From(database::common::CPluginTable::getTableName());
   BOOST_CHECK_EQUAL(beautifyQuery(test3), (boost::format("SELECT %1%,%2% FROM %3%") % database::common::CPluginTable::getIdColumnName().GetName() % database::common::CPluginTable::getDisplayNameColumnName().GetName() % database::common::CPluginTable::getTableName().GetName()).str());

   database::pgsql::CPgsqlQuery test4;
   test4.SelectCount().From(database::common::CPluginTable::getTableName());
   BOOST_CHECK_EQUAL(beautifyQuery(test4), (boost::format("SELECT COUNT(*) FROM %1%") % database::common::CPluginTable::getTableName().GetName()).str());

   database::pgsql::CPgsqlQuery test5;
   test5.SelectCount(database::common::CPluginTable::getIdColumnName()).From(database::common::CPluginTable::getTableName());
   BOOST_CHECK_EQUAL(beautifyQuery(test5), (boost::format("SELECT COUNT(%1%) FROM %2%") % database::common::CPluginTable::getIdColumnName().GetName() % database::common::CPluginTable::getTableName().GetName()).str());

   database::pgsql::CPgsqlQuery test6;
   test6.Select(test6.math(database::common::CPluginTable::getIdColumnName(), "+", 5)).From(database::common::CPluginTable::getTableName());
   BOOST_CHECK_EQUAL(beautifyQuery(test6), (boost::format("SELECT %1% + 5 FROM %2%") % database::common::CPluginTable::getIdColumnName().GetName() % database::common::CPluginTable::getTableName().GetName()).str());

   database::pgsql::CPgsqlQuery test7;
   test7.Select(test7.math(database::common::CPluginTable::getIdColumnName(), "||", ",")).From(database::common::CPluginTable::getTableName());
   BOOST_CHECK_EQUAL(beautifyQuery(test7), (boost::format("SELECT %1% || ',' FROM %2%") % database::common::CPluginTable::getIdColumnName().GetName() % database::common::CPluginTable::getTableName().GetName()).str());

   database::pgsql::CPgsqlQuery test8;
   test8.Select(test8.math(database::common::CPluginTable::getIdColumnName(), "-", database::common::CPluginTable::getDisplayNameColumnName())).From(database::common::CPluginTable::getTableName());
   BOOST_CHECK_EQUAL(beautifyQuery(test8), (boost::format("SELECT %1% - %2% FROM %3%") % database::common::CPluginTable::getIdColumnName().GetName() % database::common::CPluginTable::getDisplayNameColumnName().GetName() % database::common::CPluginTable::getTableName().GetName()).str());

}


BOOST_AUTO_TEST_CASE(InsertInto)
{
   database::pgsql::CPgsqlQuery test1;

   test1.InsertInto(database::common::CKeywordTable::getTableName(), database::common::CKeywordTable::getDeviceIdColumnName(), database::common::CKeywordTable::getCapacityNameColumnName(), database::common::CKeywordTable::getAccessModeColumnName(), database::common::CKeywordTable::getNameColumnName(), database::common::CKeywordTable::getTypeColumnName(), database::common::CKeywordTable::getMeasureColumnName()).
      Values(1, "temperature", shared::plugin::yPluginApi::EKeywordAccessMode::kGet, "test", shared::plugin::yPluginApi::EKeywordDataType::kNumeric, shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute);

   const auto check_query = (boost::format("INSERT INTO %1% (%2%,%3%,%4%,%5%,%6%,%7%) VALUES (1,'temperature','Get','test','Numeric','Absolute')") % database::common::CKeywordTable::getTableName().GetName() % database::common::CKeywordTable::getDeviceIdColumnName().GetName() % database::common::CKeywordTable::getCapacityNameColumnName().GetName() % database::common::CKeywordTable::getAccessModeColumnName().GetName() % database::common::CKeywordTable::getNameColumnName().GetName() % database::common::CKeywordTable::getTypeColumnName().GetName() % database::common::CKeywordTable::getMeasureColumnName().GetName()).str();
   BOOST_CHECK_EQUAL(beautifyQuery(test1), check_query);
}

BOOST_AUTO_TEST_CASE(InsertOrReplaceSqlite)
{
   database::sqlite::CSQLiteQuery q;

   const std::string cur_type = "Month";
   const std::string to_query = "Day";
   const auto from_date = boost::posix_time::from_iso_string("20190101T000000");
   const auto to_date = boost::posix_time::from_iso_string("20190131T000000");
   const auto keyword_id = 42;

   q.InsertOrReplaceInto(database::common::CAcquisitionSummaryTable::getTableName(), database::common::CAcquisitionSummaryTable::getTypeColumnName(),
      database::common::CAcquisitionSummaryTable::getDateColumnName(), database::common::CAcquisitionSummaryTable::getKeywordIdColumnName(),
      database::common::CAcquisitionSummaryTable::getAvgColumnName(), database::common::CAcquisitionSummaryTable::getMinColumnName(),
      database::common::CAcquisitionSummaryTable::getMaxColumnName()).
      Select(cur_type, from_date, keyword_id, q.average(q.fromSubquery("acq", database::common::CAcquisitionSummaryTable::getAvgColumnName())),
         q.min(q.fromSubquery("acq",database::common::CAcquisitionSummaryTable::getMinColumnName())),
         q.max(q.fromSubquery("acq",database::common::CAcquisitionSummaryTable::getMaxColumnName()))).
      From(q.as(database::common::CAcquisitionSummaryTable::getTableName(), "acq")).
      Where(q.fromSubquery("acq", database::common::CAcquisitionSummaryTable::getKeywordIdColumnName()), CQUERY_OP_EQUAL, keyword_id).
      And(q.fromSubquery("acq", database::common::CAcquisitionSummaryTable::getTypeColumnName()), CQUERY_OP_EQUAL, to_query).
      And(q.fromSubquery("acq", database::common::CAcquisitionSummaryTable::getDateColumnName()), CQUERY_OP_SUP_EQUAL, from_date).
      And(q.fromSubquery("acq", database::common::CAcquisitionSummaryTable::getDateColumnName()), CQUERY_OP_INF_EQUAL, to_date);

   const std::string checkQuery = "INSERT OR REPLACE INTO AcquisitionSummary (type,date,keywordId,avgValue,minValue,maxValue) SELECT 'Month','20190101T000000',42,avg(acq.avgValue),min(acq.minValue),max(acq.maxValue) FROM AcquisitionSummary AS acq WHERE acq.keywordId = 42 AND acq.type = 'Day' AND acq.date >= '20190101T000000' AND acq.date <= '20190131T000000'";
   BOOST_CHECK_EQUAL(beautifyQuery(q), checkQuery);
}



BOOST_AUTO_TEST_CASE(Update)
{
   database::pgsql::CPgsqlQuery test1;
   test1.Update(database::common::CKeywordTable::getTableName()).Set(database::common::CKeywordTable::getFriendlyNameColumnName(), "garage").
      Where(database::common::CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, 42).
      And(database::common::CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, "2.42635");

   const auto check_query = (boost::format(
         "UPDATE %1% SET friendlyName='garage' WHERE deviceId = 42 AND name = '2.42635'") % database::common::
      CKeywordTable::getTableName().GetName()).str();
   BOOST_CHECK_EQUAL(beautifyQuery(test1), check_query);

}

BOOST_AUTO_TEST_CASE(UpdateOperation)
{
   const auto check_query = (boost::format(
      "UPDATE %1% SET avgValue=avgValue * 3600, minValue=minValue * 3600, maxValue=maxValue * 3600 WHERE keywordId = 42") % database::common::CAcquisitionSummaryTable::getTableName().GetName().c_str()).str();

   database::sqlite::CSQLiteQuery q;
   // Use custom set for better performance (let do operation by the SQL engine)
   q.Update(database::common::CAcquisitionSummaryTable::getTableName()).
      Custom("SET  avgValue=avgValue * 3600, minValue=minValue * 3600, maxValue=maxValue * 3600").
      Where(database::common::CAcquisitionSummaryTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, 42);

   BOOST_CHECK_EQUAL(beautifyQuery(q), check_query);


   database::sqlite::CSQLiteQuery q2;


   q2.Update(database::common::CAcquisitionSummaryTable::getTableName()).
      Set(database::common::CAcquisitionSummaryTable::getAvgColumnName(), q2.math(database::common::CAcquisitionSummaryTable::getAvgColumnName(), CQUERY_OP_MUL, 3600),
         database::common::CAcquisitionSummaryTable::getMinColumnName(), q2.math(database::common::CAcquisitionSummaryTable::getMinColumnName(), CQUERY_OP_MUL, 3600),
         database::common::CAcquisitionSummaryTable::getMaxColumnName(), q2.math(database::common::CAcquisitionSummaryTable::getMaxColumnName(), CQUERY_OP_MUL, 3600)).
      Where(database::common::CAcquisitionSummaryTable::getKeywordIdColumnName(), CQUERY_OP_EQUAL, 42);
   BOOST_CHECK_EQUAL(beautifyQuery(q2), check_query);

}


BOOST_AUTO_TEST_CASE(WhereIn)
{
   const std::vector<int> intVector = { 3, 8, 25, 7, 12 };
   database::pgsql::CPgsqlQuery intVectorPgsqlQuery;
   intVectorPgsqlQuery.Select("column1").From("table").Where("column2", CQUERY_OP_IN, intVector);
   BOOST_CHECK_EQUAL(beautifyQuery(intVectorPgsqlQuery), "SELECT 'column1' FROM 'table' WHERE 'column2' IN ( 3, 8, 25, 7, 12)");
   database::sqlite::CSQLiteQuery intVectorSQLiteQuery;
   intVectorSQLiteQuery.Select("column1").From("table").Where("column2", CQUERY_OP_IN, intVector);
   BOOST_CHECK_EQUAL(beautifyQuery(intVectorSQLiteQuery), "SELECT 'column1' FROM 'table' WHERE 'column2' IN ( 3, 8, 25, 7, 12)");

   const std::vector<std::string> strVector = { "value1", "value2", "value3" };
   database::pgsql::CPgsqlQuery strVectorPgsqlQuery;
   strVectorPgsqlQuery.Select("column1").From("table").Where("column2", CQUERY_OP_IN, strVector);
   BOOST_CHECK_EQUAL(beautifyQuery(strVectorPgsqlQuery), "SELECT 'column1' FROM 'table' WHERE 'column2' IN ( 'value1', 'value2', 'value3')");
   database::sqlite::CSQLiteQuery strVectorSQLiteQuery;
   strVectorSQLiteQuery.Select("column1").From("table").Where("column2", CQUERY_OP_IN, strVector);
   BOOST_CHECK_EQUAL(beautifyQuery(strVectorSQLiteQuery), "SELECT 'column1' FROM 'table' WHERE 'column2' IN ( 'value1', 'value2', 'value3')");

   const std::set<int> intSet = { 3, 8, 25, 7, 12 };
   database::pgsql::CPgsqlQuery intSetPgsqlQuery;
   intSetPgsqlQuery.Select("column1").From("table").Where("column2", CQUERY_OP_IN, intSet);
   BOOST_CHECK_EQUAL(beautifyQuery(intSetPgsqlQuery), "SELECT 'column1' FROM 'table' WHERE 'column2' IN ( 3, 7, 8, 12, 25)");
   database::sqlite::CSQLiteQuery intSetSQLiteQuery;
   intSetSQLiteQuery.Select("column1").From("table").Where("column2", CQUERY_OP_IN, intSet);
   BOOST_CHECK_EQUAL(beautifyQuery(intSetSQLiteQuery), "SELECT 'column1' FROM 'table' WHERE 'column2' IN ( 3, 7, 8, 12, 25)");
}


BOOST_AUTO_TEST_SUITE_END()
