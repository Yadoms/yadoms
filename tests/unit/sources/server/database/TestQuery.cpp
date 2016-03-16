#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/enumeration/EnumHelpers.hpp"
#include "../../../../sources/server/database/entities/Entities.h"
#include "../../../../sources/shared/shared/enumeration/IExtendedEnum.h"
#include "../../../../sources/server/database/common/Query.h"
#include "../../../../sources/server/database/pgsql/PgsqlQuery.h"
#include "../../../../sources/server/database/common/DatabaseTables.h"



BOOST_AUTO_TEST_SUITE(TestQuery)

bool BothAreSpaces(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' '); }


std::string beautifyQuery(database::common::CQuery & q)
{
   //trim spaces
   std::string trimm = boost::trim_copy(q.str());
   
   //replace any double space by only one
   std::string::iterator new_end = std::unique(trimm.begin(), trimm.end(), BothAreSpaces);
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

   std::string checkQuery = (boost::format("INSERT INTO %1% (%2%,%3%,%4%,%5%,%6%,%7%) VALUES (1,'temperature','Get','test','Numeric','Absolute')") % database::common::CKeywordTable::getTableName().GetName() % database::common::CKeywordTable::getDeviceIdColumnName().GetName() % database::common::CKeywordTable::getCapacityNameColumnName().GetName() % database::common::CKeywordTable::getAccessModeColumnName().GetName() % database::common::CKeywordTable::getNameColumnName().GetName() % database::common::CKeywordTable::getTypeColumnName().GetName() % database::common::CKeywordTable::getMeasureColumnName().GetName()).str();
   BOOST_CHECK_EQUAL(beautifyQuery(test1), checkQuery);


}

BOOST_AUTO_TEST_CASE(Update)
{
   database::pgsql::CPgsqlQuery test1;
   test1.Update(database::common::CKeywordTable::getTableName()).Set(database::common::CKeywordTable::getFriendlyNameColumnName(), "garage").
      Where(database::common::CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, 42).
      And(database::common::CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, "2.42635");

   std::string checkQuery = (boost::format("UPDATE %1% SET friendlyName = 'garage' WHERE deviceId = 42 AND name = '2.42635'") % database::common::CKeywordTable::getTableName().GetName()).str();
   BOOST_CHECK_EQUAL(beautifyQuery(test1), checkQuery);

}


BOOST_AUTO_TEST_SUITE_END()
