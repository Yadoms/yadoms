#include "stdafx.h"
#include "Device.h"
#include "RestEndPoint.h"
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
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "devices", getDevices));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, "devices/{id}", getDevices));

            //TODO RAF
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("compatibleForMergeDevice"), getCompatibleForMergeDevice)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("configurationSchema"), getDeviceConfigurationSchema)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword"), getAllKeywords)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*"), getKeyword)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("matchcapacity")("*")("*"), getDevicesWithCapacity)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("matchcapacitytype")("*")("*"), getDeviceWithCapacityType)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("matchkeywordaccess")("*"), getDeviceWithKeywordAccessMode)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("matchkeywordhistorydepth")("*"), getDeviceWithKeywordHistoryDepth)
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("matchkeywordcriteria"), getDeviceMatchKeywordCriteria, transactionalMethod)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("*")("*"), getDeviceKeywordsForCapacity)
            //REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("keyword"), CDevice::getDeviceKeywords)
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("keywordslastvalue"), getKeywordsLastState, transactionalMethod)
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*")("configuration"), updateDeviceConfiguration, transactionalMethod)
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("merge"), mergeDevices, transactionalMethod)
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*")("restore"), restoreDevice, transactionalMethod)
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("keyword")("*"), updateKeywordFriendlyName, transactionalMethod)
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("keyword")("*")("blacklist"), updateKeywordBlacklist, transactionalMethod)
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("keyword")("*")("command"), sendKeywordCommand, transactionalMethod)
            //REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)("*")("*"), deleteDevice, transactionalMethod)

            //TODO RAF
            //TODO : Initialement dans le service plugin, à déplacer ici : REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("*")("createDevice"), CPlugin::createDevice, CPlugin::transactionalMethod)
            //TODO : Initialement dans le service plugin, à déplacer ici : REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("*")("deviceExtraQuery")("*")("*"), CPlugin::sendDeviceExtraQuery, CPlugin::transactionalMethod)


            return m_endPoints;
         }

         boost::shared_ptr<IAnswer> CDevice::getDevices(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               // ID
               const auto deviceId = request->pathVariable("id", std::string());
               std::vector<boost::shared_ptr<database::entities::CDevice>> devices;

               // Filtering
               const auto withBlacklisted = request->queryParamExists("with-blacklisted");
               const auto fromPluginInstanceId = request->queryParam("from-plugin-instance",
                                                                     std::string());

               if (deviceId.empty())
               {
                  devices = fromPluginInstanceId.empty()
                               ? m_deviceRequester->getDevices(withBlacklisted)
                               : m_dataProvider->getDeviceRequester()->getDevices(static_cast<int>(std::stol(fromPluginInstanceId)),
                                                                                  withBlacklisted);
               }
               else
               {
                  const auto device = m_deviceRequester->getDevice(std::stol(deviceId),
                                                                   withBlacklisted);
                  if (!fromPluginInstanceId.empty())
                  {
                     if (device->PluginId() != static_cast<int>(std::stol(fromPluginInstanceId)))
                        return boost::make_shared<CNoContentAnswer>();
                  }
                  devices.push_back(device);
               }

               // Get requested props
               const auto props = request->queryParamAsList("prop");
               std::vector<boost::shared_ptr<shared::CDataContainer>> deviceEntries;
               for (const auto& device : devices)
               {
                  auto deviceEntry = boost::make_shared<shared::CDataContainer>();
                  if (props->empty() || props->find("id") != props->end())
                     deviceEntry->set("id", device->Id());
                  if (props->empty() || props->find("plugin-id") != props->end())
                     deviceEntry->set("plugin-id", device->PluginId());
                  if (props->empty() || props->find("name") != props->end())
                     deviceEntry->set("name", device->Name());
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

               if (deviceEntries.empty())
                  return boost::make_shared<CNoContentAnswer>();

               shared::CDataContainer container;
               container.set("devices", deviceEntries);
               return boost::make_shared<CSuccessAnswer>(container);
            }

            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get devices");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 
