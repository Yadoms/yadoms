#include "stdafx.h"

#include <map>
#include <regex>
#include <string>

#include "ConfigurationMerger.h"
#include "Device.h"
#include "RestEndPoint.h"
#include "communication/callback/SynchronousCallback.h"
#include "pluginSystem/ExtraQueryData.h"
#include "pluginSystem/ManuallyDeviceCreationData.h"
#include "web/rest/CreatedAnswer.h"
#include "web/rest/ErrorAnswer.h"
#include "web/rest/Helpers.h"
#include "web/rest/NoContentAnswer.h"

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

            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "devices/", getDevicesV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "devices/{id}/", getDevicesV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "devices/{id}/extra-query/{query}", sendExtraQueryToDeviceV2));
            //TODO RAF REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("compatibleForMergeDevice"), getCompatibleForMergeDeviceV1)
            //TODO RAF REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("merge"), mergeDevicesV1, transactionalMethodV1)

            m_endPoints->push_back(MAKE_ENDPOINT(kPost, "devices", createDeviceV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kPatch, "devices/{id}", updateDeviceV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kDelete, "devices/{id}", deleteDeviceV2));

            return m_endPoints;
         }

         boost::shared_ptr<IAnswer> CDevice::getDevicesV2(const boost::shared_ptr<IRequest>& request) const
         {
            try
            {
               // ID
               const auto id = request->pathVariable("id", std::string());
               boost::optional<int> deviceId = id.empty()
                                                  ? boost::optional<int>()
                                                  : boost::make_optional(static_cast<int>(std::stol(request->pathVariable("id"))));

               // Filtering
               const auto fromPluginInstanceId = request->queryParamExists("fromPluginInstance")
                                                    ? boost::make_optional(static_cast<int>(std::stol(request->queryParam("fromPluginInstance"))))
                                                    : boost::optional<int>();
               const auto fromFriendlyName = request->queryParamExists("fromFriendlyName")
                                                ? boost::make_optional(request->queryParam("fromFriendlyName"))
                                                : boost::optional<std::string>();
               const auto fromType = request->queryParamExists("fromType")
                                        ? boost::make_optional(request->queryParam("fromType"))
                                        : boost::optional<std::string>();
               const auto fromModel = request->queryParamExists("fromModel")
                                         ? boost::make_optional(request->queryParam("fromModel"))
                                         : boost::optional<std::string>();
               const auto containsKeywordWithCapacityName = request->queryParamExists("containingKeywordWithCapacityName")
                                                               ? request->queryParamAsList("containingKeywordWithCapacityName")
                                                               : std::make_unique<std::set<std::string>>();
               const auto containsKeywordWithAccessMode = request->queryParamExists("containingKeywordWithAccessMode")
                                                             ? boost::make_optional(
                                                                shared::plugin::yPluginApi::EKeywordAccessMode(
                                                                   request->queryParam("containingKeywordWithAccessMode")))
                                                             : boost::optional<shared::plugin::yPluginApi::EKeywordAccessMode>();
               const auto containsKeywordWithDataType = request->queryParamExists("containingKeywordWithCapacityType")
                                                           ? CHelpers::convertToEnumSet<shared::plugin::yPluginApi::EKeywordDataType>(
                                                              request->queryParamAsList("containingKeywordWithCapacityType"))
                                                           : std::make_unique<std::set<shared::plugin::yPluginApi::EKeywordDataType>>();
               const auto containsKeywordWithHistoryDepth = request->queryParamExists("containingKeywordWithHistoryDepth")
                                                               ? boost::make_optional(
                                                                  shared::plugin::yPluginApi::EHistoryDepth(
                                                                     request->queryParam("containingKeywordWithHistoryDepth")))
                                                               : boost::optional<shared::plugin::yPluginApi::EHistoryDepth>();
               const auto withBlacklisted = request->queryParamExists("withBlacklisted");

               // Pagination
               const auto page = request->queryParamExists("page")
                                    ? boost::make_optional(static_cast<int>(std::stol(request->queryParam("page"))))
                                    : boost::optional<int>();
               const auto pageSize = request->queryParamExists("perPage")
                                        ? boost::make_optional(static_cast<int>(std::stol(request->queryParam("perPage"))))
                                        : boost::optional<int>();

               boost::shared_ptr<IAnswer> answer;

               // Process the request
               m_deviceRequester->getDevices(
                  deviceId,
                  fromPluginInstanceId,
                  fromFriendlyName,
                  fromType,
                  fromModel,
                  *containsKeywordWithCapacityName,
                  containsKeywordWithAccessMode,
                  *containsKeywordWithDataType,
                  containsKeywordWithHistoryDepth,
                  withBlacklisted,
                  page,
                  pageSize,
                  [this, &deviceId, &request, &answer, &page, &pageSize](const auto& devices,
                                                                         const int pagesCount)
                  {
                     if (devices.empty())
                     {
                        answer = boost::make_shared<CNoContentAnswer>();
                        return;
                     }

                     // Get requested props
                     const auto props = request->queryParamAsList("prop");
                     std::vector<boost::shared_ptr<shared::CDataContainer>> deviceEntries;
                     for (const auto& device : devices)
                     {
                        auto deviceEntry = boost::make_shared<shared::CDataContainer>();
                        if (props->empty() || props->find("id") != props->end())
                           deviceEntry->set("id", device->Id());
                        if (props->empty() || props->find("pluginInstance") != props->end())
                           deviceEntry->set("pluginInstance", device->PluginId());
                        if (props->empty() || props->find("friendlyName") != props->end())
                           deviceEntry->set("friendlyName", device->FriendlyName());
                        if (props->empty() || props->find("model") != props->end())
                           deviceEntry->set("model", device->Model());
                        if (props->empty() || props->find("details") != props->end())
                           deviceEntry->set("details", device->Details());
                        if (props->empty() || props->find("configuration") != props->end())
                           deviceEntry->set("configuration", getDeviceConfiguration(device,
                                                                                    request->acceptLanguages()));
                        if (props->empty() || props->find("type") != props->end())
                           deviceEntry->set("type", device->Type());
                        if (props->empty() || props->find("blacklisted") != props->end())
                           deviceEntry->set("blacklisted", device->Blacklist());

                        deviceEntries.push_back(deviceEntry);
                     }

                     boost::optional<CPaging> paging;
                     if (page && pageSize)
                        paging = CPaging(*page, pagesCount, *pageSize);


                     answer = CHelpers::formatGetMultiItemsAnswer(deviceId.has_value(),
                                                                  deviceEntries,
                                                                  "devices",
                                                                  paging);
                  });

               return answer;
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

         boost::shared_ptr<shared::CDataContainer> CDevice::getDeviceConfiguration(const boost::shared_ptr<database::entities::CDevice>& device,
                                                                                   const std::vector<std::string>& locales) const
         {
            try
            {
               auto schema = getDeviceConfigurationSchema(device,
                                                          getDeviceConfigurationLabels(device, locales));

               if (schema->containsChild("error")) //TODO c'est pas plutôt schema->empty() ?
                  return schema;

               if (!schema->containsChild("content"))
                  return schema;
               const auto contentNode = schema->getChild("content");

               if (device->Configuration()->empty())
                  return schema;

               return CConfigurationMerger::mergeConfigurationAndSchema(*contentNode,
                                                                        *device->Configuration());
            }
            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Fail to get device configuration : " << exception.what();
               return shared::CDataContainer::EmptyContainerSharedPtr;
            }
         }

         boost::shared_ptr<shared::CDataContainer> CDevice::getDeviceConfigurationSchema(
            const boost::shared_ptr<database::entities::CDevice>& device,
            const boost::shared_ptr<const shared::CDataContainer>& locales) const
         {
            try
            {
               auto schema = getDeviceStaticConfigurationSchema(device,
                                                                locales);
               YADOMS_LOG(debug) << "device " + device->FriendlyName(); //TODO virer
               YADOMS_LOG(debug) << schema->serialize(); //TODO virer
               if (!schema->empty())
                  return schema;

               schema = getDeviceDynamicConfigurationSchema(device,
                                                            locales);
               if (!schema->empty())
                  return schema;

               return shared::CDataContainer::make();
            }

            catch (const std::exception& exception)
            {
               YADOMS_LOG(error) << "Error processing getDevices request : " << exception.what();
               throw std::runtime_error("Fail to get device configuration schema");
            }
         }

         boost::shared_ptr<const shared::plugin::information::IInformation> CDevice::findPluginInformation(
            const boost::shared_ptr<database::entities::CDevice>& device) const
         {
            const auto pluginType = m_dataProvider->getPluginRequester()->getInstance(device->PluginId())->Type();
            const auto plugins = m_pluginManager->getPluginList();
            const auto pluginInformation = std::find_if(
               plugins.begin(),
               plugins.end(),
               [this, &pluginType](const auto& plugin)
               {
                  return plugin.second->getType() == pluginType;
               });

            if (pluginInformation == plugins.end())
               throw std::invalid_argument("No plugin associated with device " + device->FriendlyName() + " was found");

            return pluginInformation->second;
         }

         boost::shared_ptr<shared::CDataContainer> CDevice::getDeviceStaticConfigurationSchema(
            const boost::shared_ptr<database::entities::CDevice>& device,
            const boost::shared_ptr<const shared::CDataContainer>& locales) const //TODO locales à supprimer ?
         {
            const auto staticConfigurationSchemaNode = findPluginInformation(device)->getDeviceStaticConfigurationSchema();
            if (staticConfigurationSchemaNode->empty())
               return shared::CDataContainer::EmptyContainerSharedPtr;

            for (const auto& node : staticConfigurationSchemaNode->getAsMap<boost::shared_ptr<shared::CDataContainer>>("schemas"))
            {
               if (node.second->containsChild("types." + device->Type()))
               {
                  auto schema = shared::CDataContainer::make();
                  schema->set("canBeCreatedManually", node.second->getWithDefault<bool>("types." + device->Type() + ".canBeCreatedManually", false));
                  schema->set("content", node.second->getChild("content"));
                  return schema;
               }
            }

            return shared::CDataContainer::EmptyContainerSharedPtr;
         }

         boost::shared_ptr<shared::CDataContainer> CDevice::getDeviceDynamicConfigurationSchema(
            const boost::shared_ptr<database::entities::CDevice>& device,
            const boost::shared_ptr<const shared::CDataContainer>& locales) const
         {
            const auto dynamicConfigurationSchemaNode = findPluginInformation(device)->getDeviceDynamicConfigurationSchema();
            if (dynamicConfigurationSchemaNode->empty())
               return shared::CDataContainer::EmptyContainerSharedPtr;

            //TODO tout retester
            if (!m_pluginManager->isInstanceRunning(device->PluginId()))
               return shared::CDataContainer::make(std::map<std::string, std::string>(
                  {{"error", "Fail to get device configuration schema, plugin is not running"}}));

            communication::callback::CSynchronousCallback<boost::shared_ptr<shared::CDataContainer>> cb;
            m_messageSender.sendDeviceConfigurationSchemaRequest(device->Id(), cb);

            switch (cb.waitForResult())
            {
            case communication::callback::CSynchronousCallback<boost::shared_ptr<shared::CDataContainer>>::kResult:
               {
                  const auto res = cb.getCallbackResult();
                  if (!res.success)
                     return shared::CDataContainer::make(std::map<std::string, std::string>(
                        {{"error", "Fail to get device configuration schema, plugin returned error : " + res.errorMessage()}}));

                  YADOMS_LOG(trace) << res.result(); //TODO virer

                  const auto schema = res.result()->copy();

                  if (schema->empty())
                     return shared::CDataContainer::make();

                  if (!locales->empty() && locales->exists("configurationSchema"))
                     schema->mergeFrom(locales->getChild("configurationSchema"));

                  return schema;
               }
            case shared::event::kTimeout:
               {
                  return shared::CDataContainer::make(std::map<std::string, std::string>(
                     {{"error", "Fail to get device configuration schema, timeout waiting plugin answer"}}));
               }
            default:
               {
                  return shared::CDataContainer::make(std::map<std::string, std::string>(
                     {{"error", "Fail to get device configuration schema, unknown error"}}));
               }
            }
         }

         boost::shared_ptr<shared::CDataContainer> CDevice::getDeviceConfigurationLabels(boost::shared_ptr<database::entities::CDevice> device,
                                                                                         const std::vector<std::string>& locales) const
         {
            //TODO
            return shared::CDataContainer::make();
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

                     return CHelpers::createLongRunningOperationAnswer(taskId);
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
            static const std::regex DeviceNamePattern("^manuallyCreatedDevice_([[:digit:]]*)$");
            const auto& devices = m_dataProvider->getDeviceRequester()->getDevices(pluginId,
                                                                                   true);
            unsigned int lastNumber = 0;
            for (const auto& device : devices)
            {
               std::smatch result;
               if (std::regex_search(device->Name(), result, DeviceNamePattern))
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
                        // TODO extract config (voir plugins)
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
               if (body->exists("friendlyName"))
                  deviceToUpdate.FriendlyName = body->get<std::string>("friendlyName");
               if (body->exists("model"))
                  deviceToUpdate.Model = body->get<std::string>("model");
               if (body->exists("configuration"))
                  // TODO extract config(voir plugins)
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
      } //namespace service
   } //namespace rest
} //namespace web 
