#include "stdafx.h"
#include "Keyword.h"
#include "database/common/adapters/DatabaseAdapters.h"
#include <shared/exception/EmptyResult.hpp>
#include <utility>
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
            : m_databaseRequester(std::move(databaseRequester))
         {
         }

         void CKeyword::addKeyword(const entities::CKeyword& newKeyword)
         {
            const auto qSelect = m_databaseRequester->newQuery();

            qSelect->SelectCount().
                     From(CKeywordTable::getTableName()).
                     Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, newKeyword.DeviceId()).
                     And(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, newKeyword.Name());

            if (m_databaseRequester->queryCount(*qSelect) == 0)
            {
               //create the database entry with needed fields
               const auto qInsert = m_databaseRequester->newQuery();
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

               const auto update = m_databaseRequester->newQuery();
               update->Update(CKeywordTable::getTableName()).Set(CKeywordTable::getFriendlyNameColumnName(), friendlyName).
                       Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, newKeyword.DeviceId()).
                       And(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, newKeyword.Name());

               if (m_databaseRequester->queryStatement(*update) <= 0)
                  throw shared::exception::CEmptyResult("Fail to update FriendlyName field");


               if (newKeyword.Details.isDefined())
               {
                  const auto updateDetails = m_databaseRequester->newQuery();
                  updateDetails->Update(CKeywordTable::getTableName()).Set(CKeywordTable::getDetailsColumnName(), newKeyword.Details()).
                                 Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, newKeyword.DeviceId()).
                                 And(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, newKeyword.Name());

                  if (m_databaseRequester->queryStatement(*updateDetails) <= 0)
                     throw shared::exception::CEmptyResult("Fail to update Details field");
               }

               if (newKeyword.Units.isDefined())
               {
                  const auto updateUnits = m_databaseRequester->newQuery();
                  updateUnits->Update(CKeywordTable::getTableName()).Set(CKeywordTable::getUnitsColumnName(), newKeyword.Units()).
                               Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, newKeyword.DeviceId()).
                               And(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, newKeyword.Name());

                  if (m_databaseRequester->queryStatement(*updateUnits) <= 0)
                     throw shared::exception::CEmptyResult("Fail to update Units field");
               }

               if (newKeyword.TypeInfo.isDefined())
               {
                  const auto updateValues = m_databaseRequester->newQuery();
                  updateValues->Update(CKeywordTable::getTableName()).Set(CKeywordTable::getTypeInfoColumnName(), newKeyword.TypeInfo()).
                                Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, newKeyword.DeviceId()).
                                And(CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, newKeyword.Name());

                  if (m_databaseRequester->queryStatement(*updateValues) <= 0)
                     throw shared::exception::CEmptyResult("Fail to update Values field");
               }

               if (newKeyword.HistoryDepth.isDefined())
               {
                  const auto updateValues = m_databaseRequester->newQuery();
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

            const auto qSelect = m_databaseRequester->newQuery();

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

            const auto qSelect = m_databaseRequester->newQuery();
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
            const auto qSelect = m_databaseRequester->newQuery();
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
            const auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
                     From(CKeywordTable::getTableName()).
                     Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId);
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            return adapter.getResults();
         }

         std::vector<boost::shared_ptr<entities::CKeyword>> CKeyword::getKeywordsMatchingCapacity(const std::string& capacity) const
         {
            adapters::CKeywordAdapter adapter;
            const auto qSelect = m_databaseRequester->newQuery();
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
            const auto query = m_databaseRequester->newQuery();
            query->Select().
                   From(CKeywordTable::getTableName()).
                   WhereTrue();

            if (!expectedKeywordTypes.empty())
               query->And(CKeywordTable::getTypeColumnName(), CQUERY_OP_IN, expectedKeywordTypes);

            if (!expectedCapacities.empty())
               query->And(CKeywordTable::getCapacityNameColumnName(), CQUERY_OP_IN, expectedCapacities);

            if (!expectedKeywordAccesses.empty())
               query->And(CKeywordTable::getAccessModeColumnName(), CQUERY_OP_IN, expectedKeywordAccesses);

            if (!expectedKeywordHistoryDepth.empty())
               query->And(CKeywordTable::getHistoryDepthColumnName(), CQUERY_OP_IN, expectedKeywordHistoryDepth);

            query->And(CKeywordTable::getBlacklistColumnName(), CQUERY_OP_EQUAL, blacklisted);

            adapters::CKeywordAdapter keywordsAdapter;
            m_databaseRequester->queryEntities(&keywordsAdapter, *query);
            return keywordsAdapter.getResults();
         }

         std::vector<boost::shared_ptr<entities::CKeyword>> CKeyword::getKeywords(
            const boost::optional<int>& keywordId,
            const boost::optional<int>& deviceId,
            const boost::optional<std::string>& friendlyName,
            const std::set<std::string>& capacityName,
            const std::set<shared::plugin::yPluginApi::EKeywordDataType>& dataType,
            const std::set<std::string>& units,
            const boost::optional<shared::plugin::yPluginApi::EKeywordAccessMode>& accessMode,
            const boost::optional<shared::plugin::yPluginApi::EMeasureType>& measure,
            const boost::optional<shared::plugin::yPluginApi::EHistoryDepth>& historyDepth,
            bool blacklistedIncluded)
         {
            const auto query = m_databaseRequester->newQuery();

            query->Select().
                   From(CKeywordTable::getTableName()).
                   WhereTrue();

            std::set<std::string> cleanedUnits;
            for (const auto& unit : units)
               cleanedUnits.emplace(unit.rfind("data.units.", 0) == 0 ? unit : "data.units." + unit);

            if (keywordId)
               query->And(CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, *keywordId);
            if (deviceId)
               query->And(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, *deviceId);
            if (friendlyName)
               query->And(CKeywordTable::getFriendlyNameColumnName(), CQUERY_OP_EQUAL, *friendlyName);
            if (!capacityName.empty())
               query->And(CKeywordTable::getCapacityNameColumnName(), CQUERY_OP_IN, capacityName);
            if (!dataType.empty())
               query->And(CKeywordTable::getTypeColumnName(), CQUERY_OP_IN, dataType);
            if (!cleanedUnits.empty())
               query->And(CKeywordTable::getUnitsColumnName(), CQUERY_OP_IN, cleanedUnits);
            if (accessMode)
               query->And(CKeywordTable::getAccessModeColumnName(), CQUERY_OP_EQUAL, *accessMode);
            if (measure)
               query->And(CKeywordTable::getMeasureColumnName(), CQUERY_OP_EQUAL, *measure);
            if (historyDepth)
               query->And(CKeywordTable::getHistoryDepthColumnName(), CQUERY_OP_EQUAL, *historyDepth);
            if (blacklistedIncluded)
               query->And(CKeywordTable::getBlacklistColumnName(), CQUERY_OP_EQUAL, blacklistedIncluded ? 1 : 0);

            adapters::CKeywordAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *query);

            return adapter.getResults();
         }

         std::vector<boost::shared_ptr<entities::CKeyword>> CKeyword::getAllKeywords() const
         {
            adapters::CKeywordAdapter adapter;
            const auto qSelect = m_databaseRequester->newQuery();
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
            const auto qSelect = m_databaseRequester->newQuery();
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
            const auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select(CKeywordTable::getLastAcquisitionValueColumnName(), CKeywordTable::getLastAcquisitionDateColumnName()).
                     From(CKeywordTable::getTableName()).
                     Where(CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);

            adapters::CKeywordAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);

            if (!adapter.getResults().empty())
            {
               const auto result = adapter.getResults()[0];

               // Map result to acquisition entity
               auto acquisition = boost::make_shared<entities::CAcquisition>();
               acquisition->KeywordId = keywordId;
               acquisition->Date = result->LastAcquisitionDate();
               acquisition->Value = result->LastAcquisitionValue();
               return acquisition;
            }

            if (throwIfNotExists)
               throw shared::exception::CEmptyResult(
                  (boost::format("Cannot retrieve any acquisition for the keyword id=%1% in database") % keywordId).str());

            return boost::shared_ptr<entities::CAcquisition>();
         }

         std::string CKeyword::getKeywordLastData(int keywordId,
                                                  bool throwIfNotExists)
         {
            const auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select(CKeywordTable::getLastAcquisitionValueColumnName()).
                     From(CKeywordTable::getTableName()).
                     Where(CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);

            adapters::CKeywordAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);

            if (!adapter.getResults().empty())
               return adapter.getResults()[0]->LastAcquisitionValue();

            if (throwIfNotExists)
               throw shared::exception::CEmptyResult(
                  (boost::format("Cannot retrieve any acquisition for the keyword id=%1% in database") % keywordId).str());

            return std::string();
         }

         std::vector<boost::tuple<int, std::string>> CKeyword::getKeywordListLastData(const std::vector<int>& keywordIds)
         {
            const auto qSelect = m_databaseRequester->newQuery();
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
            const auto qUpdate = m_databaseRequester->newQuery();
            qUpdate->Update(CKeywordTable::getTableName()).
                     Set(CKeywordTable::getBlacklistColumnName(), blacklist).
                     Where(CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);

            if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
               throw shared::exception::CEmptyResult("Fail to update keyword blacklist state");
         }


         void CKeyword::removeKeyword(const int keywordId)
         {
            //delete keyword
            const auto qDelete = m_databaseRequester->newQuery();
            qDelete->DeleteFrom(CKeywordTable::getTableName()).
                     Where(CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);
            if (m_databaseRequester->queryStatement(*qDelete) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }

         void CKeyword::updateKeywordFriendlyName(int keywordId,
                                                  const std::string& newFriendlyName)
         {
            //get a good name
            if (newFriendlyName.empty())
               return;

            const auto keywordToUpdate = getKeyword(keywordId);
            if (!keywordToUpdate)
               throw shared::exception::CEmptyResult("Can not find keyword");

            //insert in db
            const auto qUpdate = m_databaseRequester->newQuery();
            qUpdate->Update(CKeywordTable::getTableName()).
                     Set(CKeywordTable::getFriendlyNameColumnName(), newFriendlyName).
                     Where(CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);

            if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
               throw shared::exception::CEmptyResult("Fail to update keyword friendlyName");
         }

         void CKeyword::updateLastValue(int keywordId,
                                        const boost::posix_time::ptime& valueDatetime,
                                        const std::string& value)
         {
            const auto qUpdate = m_databaseRequester->newQuery();
            qUpdate->Update(CKeywordTable::getTableName())
                   .Set(CKeywordTable::getLastAcquisitionDateColumnName(), valueDatetime,
                        CKeywordTable::getLastAcquisitionValueColumnName(), value)
                   .Where(CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);

            if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
               throw shared::exception::CEmptyResult("Fail to update keyword last value");
         }

         void CKeyword::updateKeywordName(int keywordId,
                                          const std::string& newName)
         {
            const auto qUpdate = m_databaseRequester->newQuery();
            qUpdate->Update(CKeywordTable::getTableName()).
                     Set(CKeywordTable::getNameColumnName(), newName).
                     Where(CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);

            if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
               throw shared::exception::CEmptyResult("Fail to update keyword name");
         }

         void CKeyword::updateKeyword(const entities::CKeyword& keyword) const
         {
            if (!keyword.Id.isDefined())
               throw CDatabaseException("Need an id to update");

            const auto query = m_databaseRequester->newQuery();
            query->Update(CKeywordTable::getTableName());

            if (keyword.DeviceId.isDefined())
               query->MultiSet(CKeywordTable::getDeviceIdColumnName(), keyword.DeviceId());
            if (keyword.CapacityName.isDefined())
               query->MultiSet(CKeywordTable::getCapacityNameColumnName(), keyword.CapacityName());
            if (keyword.AccessMode.isDefined())
               query->MultiSet(CKeywordTable::getAccessModeColumnName(), keyword.AccessMode());
            if (keyword.Name.isDefined())
               query->MultiSet(CKeywordTable::getNameColumnName(), keyword.Name());
            if (keyword.FriendlyName.isDefined())
               query->MultiSet(CKeywordTable::getFriendlyNameColumnName(), keyword.FriendlyName());
            if (keyword.Type.isDefined())
               query->MultiSet(CKeywordTable::getTypeColumnName(), keyword.Type());
            if (keyword.Units.isDefined())
               query->MultiSet(CKeywordTable::getUnitsColumnName(), keyword.Units());
            if (keyword.TypeInfo.isDefined())
               query->MultiSet(CKeywordTable::getTypeInfoColumnName(), keyword.TypeInfo());
            if (keyword.Measure.isDefined())
               query->MultiSet(CKeywordTable::getMeasureColumnName(), keyword.Measure());
            if (keyword.Details.isDefined())
               query->MultiSet(CKeywordTable::getDetailsColumnName(), keyword.Details());
            if (keyword.Blacklist.isDefined())
               query->MultiSet(CKeywordTable::getBlacklistColumnName(), keyword.Blacklist());
            if (keyword.LastAcquisitionValue.isDefined())
               query->MultiSet(CKeywordTable::getLastAcquisitionValueColumnName(), keyword.LastAcquisitionValue());
            if (keyword.LastAcquisitionDate.isDefined())
               query->MultiSet(CKeywordTable::getLastAcquisitionDateColumnName(), keyword.LastAcquisitionDate());
            if (keyword.HistoryDepth.isDefined())
               query->MultiSet(CKeywordTable::getHistoryDepthColumnName(), keyword.HistoryDepth());

            query->Where(CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keyword.Id());

            if (m_databaseRequester->queryStatement(*query) <= 0)
               throw shared::exception::CEmptyResult("Fail to update keyword");
         }
      } //namespace requesters
   } //namespace common
} //namespace database 
