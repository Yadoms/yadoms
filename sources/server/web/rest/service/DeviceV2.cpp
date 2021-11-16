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
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "devices", getDevicesV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "devices/{id}", getDevicesV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "devices/{id}/configuration-schema", getDeviceConfigurationSchemaV2)); //TODO à tester

            //TODO RAF
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("compatibleForMergeDevice"), getCompatibleForMergeDeviceV1)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("configurationSchema"), getDeviceConfigurationSchemaV1)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword"), getAllKeywordsV1)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*"), getKeywordV1)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("*")("*"), getDeviceKeywordsForCapacityV1)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("keyword"), CDevice::getDeviceKeywordsV1)
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("keywordslastvalue"), getKeywordsLastStateV1, transactionalMethodV1)
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*")("configuration"), updateDeviceConfigurationV1, transactionalMethodV1)
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("merge"), mergeDevicesV1, transactionalMethodV1)
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*")("restore"), restoreDeviceV1, transactionalMethodV1)
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("keyword")("*"), updateKeywordFriendlyNameV1, transactionalMethodV1)
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("keyword")("*")("blacklist"), updateKeywordBlacklistV1, transactionalMethodV1)
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("keyword")("*")("command"), sendKeywordCommandV1, transactionalMethodV1)
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)("*")("*"), deleteDeviceV1, transactionalMethodV1)

            //TODO RAF
            //TODO : Initialement dans le service plugin, à déplacer ici : REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("*")("createDevice"), CPlugin::createDevice, CPlugin::transactionalMethodV1)
            //TODO : Initialement dans le service plugin, à déplacer ici : REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("*")("deviceExtraQuery")("*")("*"), CPlugin::sendDeviceExtraQuery, CPlugin::transactionalMethodV1)


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
               const auto containsKeywordWithCapacityType = request->queryParamExists("containing-keyword-with-capacity-type")
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
                                                                  *containsKeywordWithCapacityType,
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

         boost::shared_ptr<IAnswer> CDevice::getDeviceConfigurationSchemaV2(boost::shared_ptr<IRequest> request) const
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
      } //namespace service
   } //namespace rest
} //namespace web 
