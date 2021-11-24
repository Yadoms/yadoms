#include "stdafx.h"
#include "Device.h"
#include "RestEndPoint.h"
#include "communication/callback/SynchronousCallback.h"
#include <shared/exception/EmptyResult.hpp>

#include "pluginSystem/ExtraQueryData.h"
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
         boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> CDevice::endPoints()
         {
            if (m_endPoints != nullptr)
               return m_endPoints;

            m_endPoints = boost::make_shared<std::vector<boost::shared_ptr<IRestEndPoint>>>();

            // Devices
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "devices", getDevicesV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "devices/{id}", getDevicesV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "devices/{id}/dynamic-configuration-schema", getDeviceDynamicConfigurationSchemaV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "devices/{id}/extra-query/{query}", sendExtraQueryToDeviceV2));
            //TODO RAF REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("compatibleForMergeDevice"), getCompatibleForMergeDeviceV1)
            //TODO RAF REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("merge"), mergeDevicesV1, transactionalMethodV1)

            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "devices/create", createDeviceV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPatch, "devices/{id}", updateDeviceV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kDelete, "devices/{id}", deleteDeviceV2));

            // Keywords
            // TODO créer un service "KeywordV2" ?
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "keywords", getKeywordsV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "keywords/{ids}", getKeywordsV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "keywords/{ids}/acquisitions", getKeywordsAcquisitionsV2));
            // Param resolution{all|hour|day|month|year}
            m_endPoints->push_back(MAKE_ENDPOINT(kPatch, "keywords/{id}", updateKeywordV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "keywords/{id}/command/", sendCommandV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "keywords/{id}/command/{command}", sendCommandV2));


            return m_endPoints;
         }

         boost::shared_ptr<IAnswer> CDevice::getDevicesV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               // ID
               const auto deviceId = request->pathVariableExists("id")
                                        ? boost::make_optional(static_cast<int>(std::stol(request->pathVariable("id"))))
                                        : boost::optional<int>();

               // Filtering
               const auto fromPluginInstanceId = request->queryParamExists("from-plugin-instance")
                                                    ? boost::make_optional(static_cast<int>(std::stol(request->queryParam("from-plugin-instance"))))
                                                    : boost::optional<int>();
               const auto fromFriendlyName = request->queryParamExists("from-friendly-name")
                                                ? boost::make_optional(request->queryParam("from-friendly-name"))
                                                : boost::optional<std::string>();
               const auto fromType = request->queryParamExists("from-type")
                                        ? boost::make_optional(request->queryParam("from-type"))
                                        : boost::optional<std::string>();
               const auto fromModel = request->queryParamExists("from-model")
                                         ? boost::make_optional(request->queryParam("from-model"))
                                         : boost::optional<std::string>();
               const auto containsKeywordWithCapacityName = request->queryParamExists("containing-keyword-with-capacity-name")
                                                               ? request->queryParamAsList("containing-keyword-with-capacity-name")
                                                               : std::make_unique<std::set<std::string>>();
               const auto containsKeywordWithCapacityAccessMode = request->queryParamExists("containing-keyword-with-capacity-access-mode")
                                                                     ? boost::make_optional(
                                                                        shared::plugin::yPluginApi::EKeywordAccessMode(
                                                                           request->queryParam("containing-keyword-with-capacity-access-mode")))
                                                                     : boost::optional<shared::plugin::yPluginApi::EKeywordAccessMode>();
               const auto containsKeywordWithDataType = request->queryParamExists("containing-keyword-with-capacity-type")
                                                           ? CHelpers::convertToEnumSet<shared::plugin::yPluginApi::EKeywordDataType>(
                                                              request->queryParamAsList("containing-keyword-with-capacity-type"))
                                                           : std::make_unique<std::set<shared::plugin::yPluginApi::EKeywordDataType>>();
               const auto containsKeywordWithHistoryDepth = request->queryParamExists("containing-keyword-with-history-depth")
                                                               ? boost::make_optional(
                                                                  shared::plugin::yPluginApi::EHistoryDepth(
                                                                     request->queryParam("containing-keyword-with-history-depth")))
                                                               : boost::optional<shared::plugin::yPluginApi::EHistoryDepth>();
               const auto withBlacklisted = request->queryParamExists("with-blacklisted");

               // Process the request
               const auto devices = m_deviceRequester->getDevices(deviceId,
                                                                  fromPluginInstanceId,
                                                                  fromFriendlyName,
                                                                  fromType,
                                                                  fromModel,
                                                                  *containsKeywordWithCapacityName,
                                                                  containsKeywordWithCapacityAccessMode,
                                                                  *containsKeywordWithDataType,
                                                                  containsKeywordWithHistoryDepth,
                                                                  withBlacklisted);


               if (devices.empty())
                  return boost::make_shared<CNoContentAnswer>();

               // Get requested props
               const auto props = request->queryParamAsList("prop");
               std::vector<boost::shared_ptr<shared::CDataContainer>> deviceEntries;
               for (const auto& device : devices)
               {
                  auto deviceEntry = boost::make_shared<shared::CDataContainer>();
                  if (props->empty() || props->find("id") != props->end())
                     deviceEntry->set("id", device->Id());
                  if (props->empty() || props->find("plugin-instance") != props->end())
                     deviceEntry->set("plugin-instance", device->PluginId());
                  if (props->empty() || props->find("friendly-name") != props->end())
                     deviceEntry->set("friendly-name", device->FriendlyName());
                  if (props->empty() || props->find("model") != props->end())
                     deviceEntry->set("model", device->Model());
                  if (props->empty() || props->find("details") != props->end())
                     deviceEntry->set("details", device->Details());
                  if (props->empty() || props->find("configuration") != props->end())
                     deviceEntry->set("configuration", device->Configuration());
                  if (props->empty() || props->find("type") != props->end())
                     deviceEntry->set("type", device->Type());
                  if (props->empty() || props->find("blacklisted") != props->end())
                     deviceEntry->set("blacklisted", device->Blacklist());

                  deviceEntries.push_back(deviceEntry);
               }

               shared::CDataContainer container;
               container.set("devices", deviceEntries);
               return boost::make_shared<CSuccessAnswer>(container);
            }

            catch (const shared::exception::COutOfRange& exception)
            {
               YADOMS_LOG(error) << "Error processing getDevices request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest);
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Error processing getDevices request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get devices");
            }
         }

         boost::shared_ptr<IAnswer> CDevice::getDeviceDynamicConfigurationSchemaV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               // ID               
               if (!request->pathVariableExists("id"))
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "Device ID was not provided");
               const auto deviceId = static_cast<int>(std::stol(request->pathVariable("id")));

               //create a callback (allow waiting for result)              
               communication::callback::CSynchronousCallback<boost::shared_ptr<shared::CDataContainer>> cb;

               //send request to plugin
               m_messageSender.sendDeviceConfigurationSchemaRequest(deviceId, cb);

               //wait for result
               switch (cb.waitForResult())
               {
               case communication::callback::CSynchronousCallback<boost::shared_ptr<shared::CDataContainer>>::kResult:
                  {
                     const auto res = cb.getCallbackResult();
                     if (!res.success)
                        return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                                "Fail to get device configuration schema (plugin error)");
                     if (res.result()->empty())
                        return boost::make_shared<CNoContentAnswer>();
                     return boost::make_shared<CSuccessAnswer>(*res.result());
                  }
               case shared::event::kTimeout:
                  {
                     return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                             "Fail to get device configuration schema (timeout waiting plugin answer)");
                  }
               default:
                  {
                     return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                             "Fail to get device configuration schema (unknown error)");
                  }
               }
            }

            catch (const shared::exception::CEmptyResult& exception)
            {
               YADOMS_LOG(error) << "Error processing getDevices request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest);
            }
            catch (const shared::exception::COutOfRange& exception)
            {
               YADOMS_LOG(error) << "Error processing getDevices request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest);
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Error processing getDevices request : " << exception.what();
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get device configuration schema");
            }
         }

         boost::shared_ptr<IAnswer> CDevice::sendExtraQueryToDeviceV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               // ID
               const auto id = request->pathVariable("id", std::string());
               if (id.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "device id was not provided");
               const auto deviceId = static_cast<int>(std::stol(id));

               // Query
               const auto query = request->pathVariable("query", std::string());
               if (query.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "query to device was not provided");

               return CHelpers::transactionalMethodV2(
                  request,
                  m_dataProvider,
                  [this, &deviceId, &query](const auto& req) -> boost::shared_ptr<IAnswer>
                  {
                     const auto device = m_deviceRequester->getDevice(deviceId);

                     const auto taskId = m_messageSender.sendExtraQueryAsync(device->PluginId(),
                                                                             boost::make_shared<pluginSystem::CExtraQueryData>(query,
                                                                                req->body().empty()
                                                                                   ? shared::CDataContainer::make()
                                                                                   : shared::CDataContainer::make(req->body()),
                                                                                device->Name()));

                     if (taskId.empty())
                        return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                                "Fail to get extra query task");

                     shared::CDataContainer result;
                     result.set("taskId", taskId);
                     return boost::make_shared<CSuccessAnswer>(result);
                  });
            }

            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get available instances");
            }
         }

         std::string CDevice::generateUniqueDeviceName(const int pluginId) const
         {
            static const boost::regex DeviceNamePattern("^manuallyCreatedDevice_([[:digit:]]*)$");
            const auto& devices = m_dataProvider->getDeviceRequester()->getDevices(pluginId,
                                                                                   true);
            unsigned int lastNumber = 0;
            for (const auto& device : devices)
            {
               boost::smatch result;
               if (boost::regex_search(device->Name(), result, DeviceNamePattern))
               {
                  const auto number = std::stoul(std::string(result[1].first, result[1].second));
                  if (lastNumber < number)
                     lastNumber = number;
               }
            }

            return std::string("manuallyCreatedDevice_") + std::to_string(lastNumber + 1);
         }

         boost::shared_ptr<IAnswer> CDevice::createDeviceV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               return CHelpers::transactionalMethodV2(
                  request,
                  m_dataProvider,
                  [this](const auto& req) -> boost::shared_ptr<IAnswer>
                  {
                     auto device = boost::make_shared<database::entities::CDevice>();
                     device->fillFromSerializedString(req->body());

                     if (!device->PluginId.isDefined()
                        || !device->Name.isDefined()
                        || !device->Type.isDefined())
                        return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                                "invalid device creation request. Need at least plugin-id, name, type and configuration");

                     device->Name = this->generateUniqueDeviceName(device->PluginId());

                     try
                     {
                        // Declare device
                        device = m_deviceManager->createDevice(device);

                        // Send request to plugin
                        communication::callback::CSynchronousCallback<std::string> cb;
                        const pluginSystem::CManuallyDeviceCreationData data(device->Name(),
                                                                             device->Type(),
                                                                             device->Configuration());
                        m_messageSender.sendManuallyDeviceCreationRequest(device->PluginId(),
                                                                          data,
                                                                          cb);

                        // Wait for plugin answer
                        switch (cb.waitForResult())
                        {
                        case communication::callback::CSynchronousCallback<std::string>::kResult:
                           {
                              const auto res = cb.getCallbackResult();

                              if (!res.success)
                                 throw std::runtime_error(res.errorMessage().c_str());

                              return boost::make_shared<CCreatedAnswer>("devices/" + std::to_string(device->Id()));
                           }

                        case shared::event::kTimeout:
                           {
                              throw std::runtime_error("The plugin did not respond");
                           }

                        default:
                           {
                              throw std::runtime_error("Unknown plugin result");
                           }
                        }
                     }
                     catch (shared::exception::CException& ex)
                     {
                        YADOMS_LOG(error) << "Error creating device : " << ex.what() << ", will rollback...";

                        if (m_dataProvider->getDeviceRequester()->deviceExists(device->PluginId(),
                                                                               device->Name()))
                           m_dataProvider->getDeviceRequester()->removeDevice(device->PluginId(),
                                                                              device->Name());
                        throw;
                     }
                  });
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to create device");
            }
         }

         boost::shared_ptr<IAnswer> CDevice::updateDeviceV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               // ID
               const auto id = request->pathVariable("id", std::string());
               if (id.empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "device id was not provided");
               const auto deviceId = static_cast<int>(std::stol(id));

               if (request->body().empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "body was not provided");

               const auto body = shared::CDataContainer::make(request->body());

               if (body->empty())
                  return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                          "body was not provided");

               // Filter only client-modifiable fields
               database::entities::CDevice deviceToUpdate;
               deviceToUpdate.Id = deviceId;
               if (body->exists("friendly-name"))
                  deviceToUpdate.FriendlyName = body->get<std::string>("friendly-name");
               if (body->exists("model"))
                  deviceToUpdate.Model = body->get<std::string>("model");
               if (body->exists("configuration"))
                  deviceToUpdate.Configuration = body->get<boost::shared_ptr<shared::CDataContainer>>("configuration");
               if (body->exists("blacklist"))
                  deviceToUpdate.Blacklist = body->get<bool>("blacklist");

               m_deviceManager->updateDevice(deviceToUpdate);

               if (deviceToUpdate.Blacklist.isDefined() && deviceToUpdate.Blacklist())
                  m_deviceManager->cleanupDevice(deviceId);

               return boost::make_shared<CNoContentAnswer>();
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to update device");
            }
         }

         boost::shared_ptr<IAnswer> CDevice::deleteDeviceV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               return CHelpers::transactionalMethodV2(
                  request,
                  m_dataProvider,
                  [this](const auto& req) -> boost::shared_ptr<IAnswer>
                  {
                     // ID
                     const auto id = req->pathVariable("id", std::string());
                     if (id.empty())
                        return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kBadRequest,
                                                                "device id was not provided");
                     const auto deviceId = static_cast<int>(std::stol(id));

                     m_pluginManager->notifyDeviceRemoved(deviceId);
                     m_deviceManager->removeDevice(deviceId);

                     return boost::make_shared<CNoContentAnswer>();
                  });
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to delete device");
            }
         }

         boost::shared_ptr<IAnswer> CDevice::getKeywordsV2(const boost::shared_ptr<IRequest>& request) const
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

               shared::CDataContainer container;
               container.set("keywords", keywordEntries);
               return boost::make_shared<CSuccessAnswer>(container);
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

         boost::shared_ptr<IAnswer> CDevice::getKeywordsAcquisitionsV2(const boost::shared_ptr<IRequest>& request) const
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

         boost::shared_ptr<IAnswer> CDevice::updateKeywordV2(const boost::shared_ptr<IRequest>& request) const
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

         boost::shared_ptr<IAnswer> CDevice::sendCommandV2(const boost::shared_ptr<IRequest>& request) const
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
