#include "stdafx.h"
#include "Device.h"
#include "RestEndPoint.h"
#include "communication/callback/SynchronousCallback.h"
#include <shared/exception/EmptyResult.hpp>
#include "web/rest/ErrorAnswer.h"
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
            //TODO RAF REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("compatibleForMergeDevice"), getCompatibleForMergeDeviceV1)
            //TODO RAF REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("merge"), mergeDevicesV1, transactionalMethodV1)
            m_endPoints->push_back(MAKE_ENDPOINT(kPatch, "devices/{id}", updateDeviceV2));
            //TODO RAF REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*")("configuration"), updateDeviceConfigurationV1, transactionalMethodV1)
            //TODO RAF REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*")("restore"), restoreDeviceV1, transactionalMethodV1)
            //TODO : Initialement dans le service plugin, à déplacer ici : REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("*")("createDevice"), CPlugin::createDevice, CPlugin::transactionalMethodV1)
            //TODO : Initialement dans le service plugin, à déplacer ici : REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("*")("deviceExtraQuery")("*")("*"), CPlugin::sendDeviceExtraQuery, CPlugin::transactionalMethodV1)

            // Keywords
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "keywords", getKeywordsV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "keywords/{id}", getKeywordsV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPatch, "keywords/{id}", updateKeywordV2));
            //TODO RAF REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("keywordslastvalue"), getKeywordsLastStateV1, transactionalMethodV1)
            //TODO RAF REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("keyword")("*")("command"), sendKeywordCommandV1, transactionalMethodV1)
            //TODO RAF REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)("*")("*"), deleteDeviceV1, transactionalMethodV1)


            return m_endPoints;
         }

         boost::shared_ptr<IAnswer> CDevice::getDevicesV2(boost::shared_ptr<IRequest> request) const
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
                                                           ? convert<shared::plugin::yPluginApi::EKeywordDataType>(
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

         boost::shared_ptr<IAnswer> CDevice::getDeviceDynamicConfigurationSchemaV2(boost::shared_ptr<IRequest> request) const
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

         boost::shared_ptr<IAnswer> CDevice::updateDeviceV2(boost::shared_ptr<IRequest> request) const
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

               return boost::make_shared<CNoContentAnswer>();
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to update device");
            }
         }

         boost::shared_ptr<IAnswer> CDevice::getKeywordsV2(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               // ID
               const auto keywordId = request->pathVariableExists("id")
                                         ? boost::make_optional(static_cast<int>(std::stol(request->pathVariable("id"))))
                                         : boost::optional<int>();

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
                                            ? convert<shared::plugin::yPluginApi::EKeywordDataType>(
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
               const auto keywords = m_keywordManager->getKeywords(keywordId,
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

         boost::shared_ptr<IAnswer> CDevice::updateKeywordV2(boost::shared_ptr<IRequest> request) const
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
      } //namespace service
   } //namespace rest
} //namespace web 
