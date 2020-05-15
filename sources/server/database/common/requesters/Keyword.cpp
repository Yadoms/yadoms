#include "stdafx.h"
#include "Keyword.h"
#include "database/common/adapters/DatabaseAdapters.h"
#include <shared/exception/EmptyResult.hpp>
#include "database/common/DatabaseTables.h"
#include "database/common/Query.h"
#include "database/common/adapters/MultipleValueAdapter.hpp"

namespace database
{
   namespace common
   {
      namespace requesters
      {
         CKeyword::CKeyword(boost::shared_ptr<IDatabaseRequester> databaseRequester)
            : m_databaseRequester(databaseRequester)
         {
         }

         void CKeyword::addKeyword(const entities::CKeyword& newKeyword)
         {
            auto qSelect = m_databaseRequester->newQuery();

            qSelect->SelectCount().
                     From(CKeywordTable::getTableName()).
                     Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, newKeyword.DeviceId()).
                     And(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, newKeyword.Name());

            if (m_databaseRequester->queryCount(*qSelect) == 0)
            {
               //create the database entry with needed fields
               auto qInsert = m_databaseRequester->newQuery();
               qInsert->InsertInto(CKeywordTable::getTableName(),
                                   CKeywordTable::getDeviceIdColumnName(),
                                   CKeywordTable::getCapacityNameColumnName(),
                                   CKeywordTable::getAccessModeColumnName(),
                                   CKeywordTable::getNameColumnName(),
                                   CKeywordTable::getTypeColumnName(),
                                   CKeywordTable::getMeasureColumnName()).
                        Values(newKeyword.DeviceId(),
                               newKeyword.CapacityName(),
                               newKeyword.AccessMode(),
                               newKeyword.Name(),
                               newKeyword.Type(),
                               newKeyword.Measure());

               if (m_databaseRequester->queryStatement(*qInsert) <= 0)
                  throw shared::exception::CEmptyResult("Fail to insert keyword into table");

               //update fields
               auto friendlyName = newKeyword.Name();
               if (newKeyword.FriendlyName.isDefined())
                  friendlyName = newKeyword.FriendlyName();

               auto update = m_databaseRequester->newQuery();
               update->Update(CKeywordTable::getTableName()).Set(CKeywordTable::getFriendlyNameColumnName(), friendlyName).
                       Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, newKeyword.DeviceId()).
                       And(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, newKeyword.Name());

               if (m_databaseRequester->queryStatement(*update) <= 0)
                  throw shared::exception::CEmptyResult("Fail to update FriendlyName field");


               if (newKeyword.Details.isDefined())
               {
                  auto updateDetails = m_databaseRequester->newQuery();
                  updateDetails->Update(CKeywordTable::getTableName()).Set(CKeywordTable::getDetailsColumnName(), newKeyword.Details()).
                                 Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, newKeyword.DeviceId()).
                                 And(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, newKeyword.Name());

                  if (m_databaseRequester->queryStatement(*updateDetails) <= 0)
                     throw shared::exception::CEmptyResult("Fail to update Details field");
               }

               if (newKeyword.Units.isDefined())
               {
                  auto updateUnits = m_databaseRequester->newQuery();
                  updateUnits->Update(CKeywordTable::getTableName()).Set(CKeywordTable::getUnitsColumnName(), newKeyword.Units()).
                               Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, newKeyword.DeviceId()).
                               And(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, newKeyword.Name());

                  if (m_databaseRequester->queryStatement(*updateUnits) <= 0)
                     throw shared::exception::CEmptyResult("Fail to update Units field");
               }

               if (newKeyword.TypeInfo.isDefined())
               {
                  auto updateValues = m_databaseRequester->newQuery();
                  updateValues->Update(CKeywordTable::getTableName()).Set(CKeywordTable::getTypeInfoColumnName(), newKeyword.TypeInfo()).
                                Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, newKeyword.DeviceId()).
                                And(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, newKeyword.Name());

                  if (m_databaseRequester->queryStatement(*updateValues) <= 0)
                     throw shared::exception::CEmptyResult("Fail to update Values field");
               }

               if (newKeyword.HistoryDepth.isDefined())
               {
                  auto updateValues = m_databaseRequester->newQuery();
                  updateValues->Update(CKeywordTable::getTableName()).Set(CKeywordTable::getHistoryDepthColumnName(), newKeyword.HistoryDepth()).
                                Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, newKeyword.DeviceId()).
                                And(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, newKeyword.Name());

                  if (m_databaseRequester->queryStatement(*updateValues) <= 0)
                     throw shared::exception::CEmptyResult("Fail to update Values field");
               }
            }
            else
            {
               //already exist, do nothing
            }
         }

         boost::shared_ptr<entities::CKeyword> CKeyword::getKeyword(int deviceId,
                                                                    const std::string& keyword) const
         {
            adapters::CKeywordAdapter adapter;

            auto qSelect = m_databaseRequester->newQuery();

            qSelect->Select().
                     From(CKeywordTable::getTableName()).
                     Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId).
                     And(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, keyword);

            m_databaseRequester->queryEntities(&adapter, *qSelect);
            if (adapter.getResults().empty())
               throw shared::exception::CEmptyResult(
                  (boost::format("Keyword name %1% for device %2% not found in database") % keyword % deviceId).str());

            return adapter.getResults().at(0);
         }

         boost::shared_ptr<entities::CKeyword> CKeyword::getKeyword(int keywordId) const
         {
            adapters::CKeywordAdapter adapter;

            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
                     From(CKeywordTable::getTableName()).
                     Where(CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);

            m_databaseRequester->queryEntities(&adapter, *qSelect);
            if (adapter.getResults().empty())
               throw shared::exception::CEmptyResult((boost::format("Keyword id %1% not found in database") % keywordId).str());

            return adapter.getResults().at(0);
         }

         std::vector<boost::shared_ptr<entities::CKeyword>> CKeyword::getKeywordIdFromFriendlyName(int deviceId,
                                                                                                   const std::string& friendlyName) const
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
                     From(CKeywordTable::getTableName()).
                     Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId).
                     And(CKeywordTable::getFriendlyNameColumnName(), CQUERY_OP_EQUAL, friendlyName);

            adapters::CKeywordAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            return adapter.getResults();
         }

         std::vector<boost::shared_ptr<entities::CKeyword>> CKeyword::getKeywords(int deviceId) const
         {
            adapters::CKeywordAdapter adapter;
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
                     From(CKeywordTable::getTableName()).
                     Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId);
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            return adapter.getResults();
         }

         std::vector<boost::shared_ptr<entities::CKeyword>> CKeyword::getKeywordsMatchingCapacity(const std::string& capacity) const
         {
            adapters::CKeywordAdapter adapter;
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
                     From(CKeywordTable::getTableName()).
                     Where(CKeywordTable::getCapacityNameColumnName(), CQUERY_OP_EQUAL, capacity);
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            return adapter.getResults();
         }

         std::vector<boost::shared_ptr<entities::CKeyword>> CKeyword::getKeywordsMatchingCriteria(
            const std::vector<shared::plugin::yPluginApi::EKeywordDataType>& expectedKeywordTypes,
            const std::vector<std::string>& expectedCapacities,
            const std::vector<shared::plugin::yPluginApi::EKeywordAccessMode>& expectedKeywordAccesses,
            const std::vector<shared::plugin::yPluginApi::EHistoryDepth>& expectedKeywordHistoryDepth,
            bool blacklisted) const
         {
            auto subQuery = m_databaseRequester->newQuery();
            subQuery->Select().
               From(CKeywordTable::getTableName()).
               WhereTrue();

            if (!expectedKeywordTypes.empty())
               subQuery->And(CKeywordTable::getTypeColumnName(), CQUERY_OP_IN, expectedKeywordTypes);

            if (!expectedCapacities.empty())
               subQuery->And(CKeywordTable::getCapacityNameColumnName(), CQUERY_OP_IN, expectedCapacities);

            if (!expectedKeywordAccesses.empty())
               subQuery->And(CKeywordTable::getAccessModeColumnName(), CQUERY_OP_IN, expectedKeywordAccesses);

            if (!expectedKeywordHistoryDepth.empty())
               subQuery->And(CKeywordTable::getHistoryDepthColumnName(), CQUERY_OP_IN, expectedKeywordHistoryDepth);

            subQuery->And(CKeywordTable::getBlacklistColumnName(), CQUERY_OP_EQUAL, blacklisted);

            adapters::CKeywordAdapter keywordsAdapter;
            m_databaseRequester->queryEntities(&keywordsAdapter, *subQuery);
            return keywordsAdapter.getResults();
         }

         std::vector<boost::shared_ptr<entities::CKeyword>> CKeyword::getAllKeywords() const
         {
            adapters::CKeywordAdapter adapter;
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().From(CKeywordTable::getTableName());
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            return adapter.getResults();
         }

         std::vector<boost::shared_ptr<entities::CKeyword>> CKeyword::getDeviceKeywordsWithCapacity(int deviceId,
                                                                                                    const std::string& capacityName,
                                                                                                    const shared::plugin::yPluginApi::
                                                                                                    EKeywordAccessMode& accessMode) const
         {
            adapters::CKeywordAdapter adapter;
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
                     From(CKeywordTable::getTableName()).
                     Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId).
                     And(CKeywordTable::getCapacityNameColumnName(), CQUERY_OP_EQUAL, capacityName).
                     And(CKeywordTable::getAccessModeColumnName(), CQUERY_OP_EQUAL, accessMode);

            m_databaseRequester->queryEntities(&adapter, *qSelect);
            return adapter.getResults();
         }

         boost::shared_ptr<entities::CAcquisition> CKeyword::getKeywordLastAcquisition(const int keywordId,
                                                                                       bool throwIfNotExists)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select(CKeywordTable::getLastAcquisitionValueColumnName(), CKeywordTable::getLastAcquisitionDateColumnName()).
                     From(CKeywordTable::getTableName()).
                     Where(CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);

            adapters::CKeywordAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);

            if (!adapter.getResults().empty())
            {
               auto result = adapter.getResults()[0];

               // Map result to acquisition entity
               auto acquisition = boost::make_shared<entities::CAcquisition>();
               acquisition->KeywordId = keywordId;
               acquisition->Date = result->LastAcquisitionDate();
               acquisition->Value = result->LastAcquisitionValue();
               return acquisition;
            }
            if (throwIfNotExists)
            {
               throw shared::exception::CEmptyResult(
                  (boost::format("Cannot retrieve any acquisition for the keyword id=%1% in database") % keywordId).str());
            }
            return boost::shared_ptr<entities::CAcquisition>();
         }

         std::string CKeyword::getKeywordLastData(int keywordId,
                                                  bool throwIfNotExists)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select(CKeywordTable::getLastAcquisitionValueColumnName()).
                     From(CKeywordTable::getTableName()).
                     Where(CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);

            adapters::CKeywordAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);

            if (!adapter.getResults().empty())
            {
               return adapter.getResults()[0]->LastAcquisitionValue();
            }
            if (throwIfNotExists)
            {
               throw shared::exception::CEmptyResult(
                  (boost::format("Cannot retrieve any acquisition for the keyword id=%1% in database") % keywordId).str());
            }
            return std::string();
         }

         std::vector<boost::tuple<int, std::string>> CKeyword::getKeywordListLastData(const std::vector<int>& keywordIds)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select(CKeywordTable::getIdColumnName(), CKeywordTable::getLastAcquisitionValueColumnName()).
                     From(CKeywordTable::getTableName()).
                     Where(CKeywordTable::getIdColumnName(), CQUERY_OP_IN, keywordIds);

            adapters::CMultipleValueAdapter<int, std::string> mva;
            m_databaseRequester->queryEntities(&mva, *qSelect);

            return mva.getResults();
         }

         void CKeyword::updateKeywordBlacklistState(int keywordId,
                                                    const bool blacklist)
         {
            const auto keywordToUpdate = getKeyword(keywordId);
            if (!keywordToUpdate)
               throw shared::exception::CEmptyResult("Can not find keyword");

            //insert in db
            auto qUpdate = m_databaseRequester->newQuery();
            qUpdate->Update(CKeywordTable::getTableName()).
                     Set(CKeywordTable::getBlacklistColumnName(), blacklist).
                     Where(CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);

            if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
               throw shared::exception::CEmptyResult("Fail to update keyword blacklist state");
         }


         void CKeyword::removeKeyword(const int keywordId)
         {
            //delete keyword
            auto qDelete = m_databaseRequester->newQuery();
            qDelete->DeleteFrom(CKeywordTable::getTableName()).
                     Where(CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);
            if (m_databaseRequester->queryStatement(*qDelete) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }

         void CKeyword::updateKeywordFriendlyName(int keywordId,
                                                  const std::string& newFriendlyName)
         {
            //get a good name
            if (!newFriendlyName.empty())
            {
               const auto keywordToUpdate = getKeyword(keywordId);
               if (!keywordToUpdate)
                  throw shared::exception::CEmptyResult("Can not find keyword");

               //insert in db
               auto qUpdate = m_databaseRequester->newQuery();
               qUpdate->Update(CKeywordTable::getTableName()).
                        Set(CKeywordTable::getFriendlyNameColumnName(), newFriendlyName).
                        Where(CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);

               if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                  throw shared::exception::CEmptyResult("Fail to update keyword friendlyName");
            }
         }

         void CKeyword::updateLastValue(int keywordId,
                                        const boost::posix_time::ptime& valueDatetime,
                                        const std::string& value)
         {
            auto qUpdate = m_databaseRequester->newQuery();
            qUpdate->Update(CKeywordTable::getTableName())
                   .Set(CKeywordTable::getLastAcquisitionDateColumnName(), valueDatetime,
                        CKeywordTable::getLastAcquisitionValueColumnName(), value)
                   .Where(CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);

            if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
               throw shared::exception::CEmptyResult("Fail to update keyword last value");
         }
      } //namespace requesters
   } //namespace common
} //namespace database 
