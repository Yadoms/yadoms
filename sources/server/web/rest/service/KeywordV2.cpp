#include "stdafx.h"
#include "Keyword.h"
#include "RestEndPoint.h"
#include "communication/callback/SynchronousCallback.h"
#include <shared/exception/EmptyResult.hpp>
#include <utility>

#include "pluginSystem/ManuallyDeviceCreationData.h"
#include "web/rest/CreatedAnswer.h"
#include "web/rest/ErrorAnswer.h"
#include "web/rest/Helpers.h"
#include "web/rest/NoContentAnswer.h"
#include "web/rest/SuccessAnswer.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         CKeyword::CKeyword(boost::shared_ptr<database::IDataProvider> dataProvider,
                            boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordManager,
                            communication::ISendMessageAsync& messageSender)
            : m_dataProvider(std::move(dataProvider)),
              m_keywordManager(std::move(keywordManager)),
              m_messageSender(messageSender)
         {
         }

         boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> CKeyword::endPoints()
         {
            if (m_endPoints != nullptr)
               return m_endPoints;

            m_endPoints = boost::make_shared<std::vector<boost::shared_ptr<IRestEndPoint>>>();

            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "keywords", getKeywordsV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "keywords/{ids}", getKeywordsV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "keywords/{ids}/acquisitions", getKeywordsAcquisitionsV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPatch, "keywords/{id}", updateKeywordV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "keywords/{id}/command/", sendCommandV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "keywords/{id}/command/{command}", sendCommandV2));

            return m_endPoints;
         }

         boost::shared_ptr<IAnswer> CKeyword::getKeywordsV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               // ID
               const auto keywordIds = request->pathVariableExists("ids")
                                          ? CHelpers::convertToIntSet(request->pathVariableAsList("ids"))
                                          : std::make_unique<std::set<int>>();

               // Filtering
               const auto fromDeviceId = request->queryParamExists("from-device-id")
                                            ? boost::make_optional(static_cast<int>(std::stol(request->queryParam("from-device-id"))))
                                            : boost::optional<int>();
               const auto fromCapacityName = request->queryParamExists("from-capacity-name")
                                                ? request->queryParamAsList("from-capacity-name")
                                                : std::make_unique<std::set<std::string>>();
               const auto fromAccessMode = request->queryParamExists("from-access-mode")
                                              ? boost::make_optional(
                                                 shared::plugin::yPluginApi::EKeywordAccessMode(request->queryParam("from-access-mode")))
                                              : boost::optional<shared::plugin::yPluginApi::EKeywordAccessMode>();
               const auto fromFriendlyName = request->queryParamExists("from-friendly-name")
                                                ? boost::make_optional(request->queryParam("from-friendly-name"))
                                                : boost::optional<std::string>();
               const auto fromDataType = request->queryParamExists("from-data-type")
                                            ? CHelpers::convertToEnumSet<shared::plugin::yPluginApi::EKeywordDataType>(
                                               request->queryParamAsList("from-data-type"))
                                            : std::make_unique<std::set<shared::plugin::yPluginApi::EKeywordDataType>>();
               const auto fromUnits = request->queryParamExists("from-units")
                                         ? request->queryParamAsList("from-units")
                                         : std::make_unique<std::set<std::string>>();
               const auto fromMeasure = request->queryParamExists("from-measure")
                                           ? boost::make_optional(shared::plugin::yPluginApi::EMeasureType(request->queryParam("from-measure")))
                                           : boost::optional<shared::plugin::yPluginApi::EMeasureType>();
               const auto withBlacklisted = request->queryParamExists("with-blacklisted");
               const auto fromHistoryDepth = request->queryParamExists("from-history-depth")
                                                ? boost::make_optional(
                                                   shared::plugin::yPluginApi::EHistoryDepth(request->queryParam("from-history-depth")))
                                                : boost::optional<shared::plugin::yPluginApi::EHistoryDepth>();

               // Process the request
               const auto keywords = m_keywordManager->getKeywords(*keywordIds,
                                                                   fromDeviceId,
                                                                   fromFriendlyName,
                                                                   *fromCapacityName,
                                                                   *fromDataType,
                                                                   *fromUnits,
                                                                   fromAccessMode,
                                                                   fromMeasure,
                                                                   fromHistoryDepth,
                                                                   withBlacklisted);

               if (keywords.empty())
                  return boost::make_shared<CNoContentAnswer>();

               // Get requested props
               const auto props = request->queryParamAsList("prop");
               std::vector<boost::shared_ptr<shared::CDataContainer>> keywordEntries;
               for (const auto& keyword : keywords)
               {
                  auto keywordEntry = boost::make_shared<shared::CDataContainer>();
                  if (props->empty() || props->find("id") != props->end())
                     keywordEntry->set("id", keyword->Id());
                  if (props->empty() || props->find("device-id") != props->end())
                     keywordEntry->set("device-id", keyword->DeviceId());
                  if (props->empty() || props->find("capacity-name") != props->end())
                     keywordEntry->set("capacity-name", keyword->CapacityName());
                  if (props->empty() || props->find("access-mode") != props->end())
                     keywordEntry->set("access-mode", keyword->AccessMode());
                  if (props->empty() || props->find("friendly-name") != props->end())
                     keywordEntry->set("friendly-name", keyword->FriendlyName());
                  if (props->empty() || props->find("type") != props->end())
                     keywordEntry->set("type", keyword->Type());
                  if (props->empty() || props->find("units") != props->end())
                     keywordEntry->set("units", keyword->Units());
                  if (props->empty() || props->find("type-info") != props->end())
                     keywordEntry->set("type-info", keyword->TypeInfo());
                  if (props->empty() || props->find("measure") != props->end())
                     keywordEntry->set("measure", keyword->Measure());
                  if (props->empty() || props->find("details") != props->end())
                     keywordEntry->set("details", keyword->Details());
                  if (props->empty() || props->find("blacklisted") != props->end())
                     keywordEntry->set("blacklisted", keyword->Blacklist());
                  if (props->empty() || props->find("last-acquisition-value") != props->end())
                     keywordEntry->set("last-acquisition-value", keyword->LastAcquisitionValue());
                  if (props->empty() || props->find("last-acquisition-date") != props->end())
                     keywordEntry->set("last-acquisition-date", keyword->LastAcquisitionDate());
                  if (props->empty() || props->find("history-depth") != props->end())
                     keywordEntry->set("history-depth", keyword->HistoryDepth());

                  keywordEntries.push_back(keywordEntry);
               }

               return CHelpers::formatGetMultiItemsAnswer(keywordIds->size() == 1,
                                                          keywordEntries,
                                                          "keywords");
            }

            catch (const shared::exception::COutOfRange& exception)
            {
               YADOMS_LOG(error) << "Error processing getKeywords request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest);
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Error processing getKeywords request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get keywords");
            }
         }

         boost::shared_ptr<IAnswer> CKeyword::getKeywordsAcquisitionsV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               // ID
               const auto keywordIds = request->pathVariableExists("ids")
                                          ? CHelpers::convertToIntSet(request->pathVariableAsList("ids"))
                                          : std::make_unique<std::set<int>>();

               // Filtering
               const auto fromDate = request->queryParamExists("from-date")
                                        ? boost::posix_time::from_iso_string(request->queryParam("from-date"))
                                        : boost::posix_time::not_a_date_time;
               const auto toDate = request->queryParamExists("to-date")
                                      ? boost::posix_time::from_iso_string(request->queryParam("to-date"))
                                      : boost::posix_time::not_a_date_time;
               auto resolution = boost::optional<database::entities::EAcquisitionSummaryType>();
               if (request->queryParamExists("resolution"))
               {
                  const auto resolutionStr = request->queryParam("resolution");
                  if (resolutionStr == "hour")
                     resolution = boost::make_optional(database::entities::EAcquisitionSummaryType::kHour);
                  else if (resolutionStr == "day")
                     resolution = boost::make_optional(database::entities::EAcquisitionSummaryType::kDay);
                  else if (resolutionStr == "month")
                     resolution = boost::make_optional(database::entities::EAcquisitionSummaryType::kMonth);
                  else if (resolutionStr == "year")
                     resolution = boost::make_optional(database::entities::EAcquisitionSummaryType::kYear);
                  else if (resolutionStr != "all")
                     throw shared::exception::COutOfRange("invalid resolution value : " + resolutionStr);
               }
               const auto limit = request->queryParamExists("limit")
                                     ? static_cast<int>(std::stol(request->queryParam("limit")))
                                     : -1;

               // Get requested props
               bool average = false, min = false, max = false, count = false;
               if (resolution.has_value())
               {
                  const auto props = request->queryParamAsList("prop");
                  for (const auto& prop : *props)
                  {
                     if (prop == "average")
                        average = true;
                     else if (prop == "min")
                        min = true;
                     else if (prop == "max")
                        max = true;
                     else if (prop == "count")
                        count = true;
                  }
               }

               // Process the request
               const auto allData = resolution.has_value()
                                       ? m_dataProvider->getAcquisitionRequester()->getHugeVectorKeywordSummaryDataV2(
                                          *resolution,
                                          *keywordIds,
                                          fromDate,
                                          toDate,
                                          limit,
                                          average,
                                          min,
                                          max,
                                          count)
                                       : m_dataProvider->getAcquisitionRequester()->getHugeVectorKeywordDataV2(
                                          *keywordIds,
                                          fromDate,
                                          toDate,
                                          limit);


               if (allData.empty())
                  return boost::make_shared<CNoContentAnswer>();

               return boost::make_shared<CSuccessAnswer>(allData,
                                                         EContentType::kJson);
            }

            catch (const shared::exception::COutOfRange& exception)
            {
               YADOMS_LOG(error) << "Error processing getKeywords request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest);
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Error processing getKeywords request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get keywords");
            }
         }

         boost::shared_ptr<IAnswer> CKeyword::updateKeywordV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               // ID
               const auto id = request->pathVariable("id", std::string());
               if (id.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "keyword id was not provided");
               const auto keywordId = static_cast<int>(std::stol(id));

               if (request->body().empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "body was not provided");

               const auto body = shared::CDataContainer::make(request->body());

               if (body->empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "body was not provided");

               // Filter only client-modifiable fields
               database::entities::CKeyword keywordToUpdate;
               keywordToUpdate.Id = keywordId;
               if (body->exists("friendly-name"))
                  keywordToUpdate.FriendlyName = body->get<std::string>("friendly-name");
               if (body->exists("blacklist"))
                  keywordToUpdate.Blacklist = body->get<bool>("blacklist");

               m_keywordManager->updateKeyword(keywordToUpdate);

               return boost::make_shared<CNoContentAnswer>();
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to update keyword");
            }
         }

         boost::shared_ptr<IAnswer> CKeyword::sendCommandV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               // ID
               const auto id = request->pathVariable("id", std::string());
               if (id.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "plugin-instance id was not provided");
               const auto keywordId = static_cast<int>(std::stol(id));

               m_messageSender.sendKeywordCommandAsync(keywordId, request->pathVariable("command", std::string()));
               return boost::make_shared<CNoContentAnswer>();
            }

            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get available instances");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 
