#include "stdafx.h"
#include "Device.h"
#include <shared/exception/EmptyResult.hpp>
#include "web/rest/Result.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "communication/callback/SynchronousCallback.h"
#include "database/common/requesters/Keyword.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         std::string CDevice::m_restKeyword = std::string("device");


         CDevice::CDevice(boost::shared_ptr<database::IDataProvider> dataProvider,
                          boost::shared_ptr<pluginSystem::CManager> pluginManager,
                          boost::shared_ptr<dataAccessLayer::IDeviceManager> deviceManager,
                          boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordManager,
                          communication::ISendMessageAsync& messageSender)
            : m_dataProvider(dataProvider),
              m_deviceRequester(dataProvider->getDeviceRequester()),
              m_pluginManager(pluginManager),
              m_deviceManager(deviceManager),
              m_keywordManager(keywordManager),
              m_messageSender(messageSender)
         {
         }

         const std::string& CDevice::getRestKeyword()
         {
            return m_restKeyword;
         }

         void CDevice::configureDispatcher(CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CDevice::getAllDevices);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*"), CDevice::getOneDevice);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("compatibleForMergeDevice"),
                                        CDevice::getCompatibleForMergeDevice);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("configurationSchema"),
                                        CDevice::getDeviceConfigurationSchema);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword"), CDevice::getAllKeywords);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*"), CDevice::getKeyword);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("matchcapacity")("*")("*"),
                                        CDevice::getDevicesWithCapacity);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("matchcapacitytype")("*")("*"),
                                        CDevice::getDeviceWithCapacityType);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("matchkeywordaccess")("*"),
                                        CDevice::getDeviceWithKeywordAccessMode);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("matchkeywordhistorydepth")("*"),
                                        CDevice::getDeviceWithKeywordHistoryDepth
            );
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("matchkeywordcriteria"),
                                                        CDevice::getDeviceMatchKeywordCriteria,
                                                        CDevice::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("matchdevicecriteria"),
                                                        CDevice::getDeviceMatchDeviceCriteria,
                                                        CDevice::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("*")("*"),
                                        CDevice::getDeviceKeywordsForCapacity);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("keyword"), CDevice::getDeviceKeywords);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("keywordslastvalue"),
                                                        CDevice::getKeywordsLastState,
                                                        CDevice:: transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*")("configuration"),
                                                        CDevice::updateDeviceConfiguration,
                                                        CDevice::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("merge"),
                                                        CDevice::mergeDevices,
                                                        CDevice:: transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*")("restore"),
                                                        CDevice::restoreDevice,
                                                        CDevice:: transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("keyword")("*"),
                                                        CDevice::updateKeywordFriendlyName,
                                                        CDevice::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("keyword")("*")("blacklist"),
                                                        CDevice:: updateKeywordBlacklist, CDevice::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("keyword")("*")("command"),
                                                        CDevice::sendKeywordCommand,
                                                        CDevice::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)("*")("*"),
                                                        CDevice::deleteDevice,
                                                        CDevice:: transactionalMethod);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CDevice::getOneDevice(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            std::string objectId;
            if (parameters.size() > 1)
               objectId = parameters[1];

            const auto deviceFound = m_deviceRequester->getDevice(boost::lexical_cast<int>(objectId), true);
            return CResult::GenerateSuccess(deviceFound);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CDevice::getCompatibleForMergeDevice(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() <= 1)
                  return CResult::GenerateError("invalid parameter. Can not retrieve device id in url");

               const auto refDeviceId = boost::lexical_cast<int>(parameters[1]);

               // A device is compatible for merge with an other when :
               // - plugins are compatible (see m_deviceRequester->getCompatibleForMergeDevice)
               // - They contain at least one compatible keyword. Keywords are compatible when :
               //   - capacityName, accessMode, name, type, units, typeInfo, measure and details are the same

               // Get all devices with same characteristics of refDevice
               auto compatibleDevices = m_deviceRequester->getCompatibleForMergeDevice(refDeviceId);
               shared::CDataContainer commonKeywordsForCompatibleDevices;
               if (!compatibleDevices.empty())
               {
                  // Second filter, check keywords from already filtered devices
                  const auto refKeywords = m_keywordManager->getKeywords(refDeviceId);
                  // Remove device from compatibles devices list if no common keyword exist between reference and first-filtered device
                  compatibleDevices.erase(
                     std::remove_if(compatibleDevices.begin(),
                                    compatibleDevices.end(),
                                    [this, &refKeywords, &commonKeywordsForCompatibleDevices](
                                    const boost::shared_ptr<database::entities::CDevice>& candidateDevice)
                                    {
                                       auto candidateKeywords = m_keywordManager->getKeywords(candidateDevice->Id);
                                       std::vector<shared::CDataContainer> commonKeywords;
                                       // Iterate through reference keywords to find a common keyword in candidateDevice
                                       for (const auto& refKeyword : refKeywords)
                                       {
                                          // Iterate through candidate keywords and remove consumed candidate to not used it more than one time
                                          candidateKeywords.erase(
                                             std::remove_if(candidateKeywords.begin(),
                                                            candidateKeywords.end(),
                                                            [&commonKeywords, &refKeyword](
                                                            const boost::shared_ptr<database::entities::CKeyword>&
                                                            candidateKeyword)
                                                            {
                                                               if (refKeyword->CapacityName == candidateKeyword->
                                                                  CapacityName &&
                                                                  refKeyword->AccessMode == candidateKeyword->AccessMode
                                                                  &&
                                                                  refKeyword->Name == candidateKeyword->Name &&
                                                                  refKeyword->Type == candidateKeyword->Type &&
                                                                  refKeyword->Units == candidateKeyword->Units &&
                                                                  refKeyword->TypeInfo == candidateKeyword->TypeInfo &&
                                                                  refKeyword->Measure == candidateKeyword->Measure &&
                                                                  refKeyword->Details == candidateKeyword->Details)
                                                               {
                                                                  // A common device was found
                                                                  shared::CDataContainer commonKeyword;
                                                                  commonKeyword.set("from", refKeyword);
                                                                  commonKeyword.set("to", candidateKeyword);
                                                                  commonKeywords.push_back(commonKeyword);
                                                                  return true;
                                                               }
                                                               return false;
                                                            }),
                                             candidateKeywords.end());
                                       }

                                       if (commonKeywords.empty())
                                       {
                                          // Remove device from compatible devices list if no common keyword
                                          return true;
                                       }

                                       commonKeywordsForCompatibleDevices.set(
                                          std::to_string(candidateDevice->Id), commonKeywords);
                                       return false;
                                    }),
                     compatibleDevices.end());
               }
               shared::CDataContainer collection;
               collection.set("compatibleDevices", compatibleDevices);
               collection.set("commonKeywordsByDevice", commonKeywordsForCompatibleDevices);
               return CResult::GenerateSuccess(collection);
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in executing getDeviceConfigurationSchema query");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CDevice::getDeviceConfigurationSchema(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 2)
               {
                  const auto deviceId = boost::lexical_cast<int>(parameters[1]);
                  try
                  {
                     //create a callback (allow waiting for result)              
                     communication::callback::CSynchronousCallback<shared::CDataContainer> cb;

                     //send request to plugin
                     m_messageSender.sendDeviceConfigurationSchemaRequest(deviceId, cb);

                     //wait for result
                     switch (cb.waitForResult())
                     {
                     case communication::callback::CSynchronousCallback<shared::CDataContainer>::kResult:
                        {
                           const auto res = cb.getCallbackResult();
                           if (res.success)
                              return CResult::GenerateSuccess(res.result);
                           return CResult::GenerateError(res.errorMessage);
                        }
                     case shared::event::kTimeout:
                        return CResult::GenerateError("The plugin did not respond");
                     default:
                        return CResult::GenerateError("Unknown plugin result");
                     }
                  }
                  catch (shared::exception::CException& ex)
                  {
                     return CResult::GenerateError(ex);
                  }
               }
               return CResult::GenerateError("invalid parameter. Can not retrieve device id in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in executing getDeviceConfigurationSchema query");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CDevice::getAllDevices(
            const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            const auto dvList = m_deviceRequester->getDevices(true);
            shared::CDataContainer collection;
            collection.set(getRestKeyword(), dvList);
            return CResult::GenerateSuccess(collection);
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CDevice::getKeyword(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() >= 2)
               {
                  const auto keywordId = boost::lexical_cast<int>(parameters[2]);
                  const auto keyword = m_keywordManager->getKeyword(keywordId);
                  return CResult::GenerateSuccess(keyword);
               }
               return CResult::GenerateError("invalid parameter. Can not retrieve keyword id in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving keyword");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CDevice::getKeywordsLastState(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() >= 2)
               {
                  const auto keywordIds = shared::CDataContainer(requestContent);
                  const auto keywordListLastData = m_keywordManager->getKeywordListLastData(
                     keywordIds.get<std::vector<int>>("keywordIds"));
                  shared::CDataContainer result;
                  for (auto keywordLastData : keywordListLastData)
                  {
                     result.set(std::to_string(keywordLastData.get<0>()), keywordLastData.get<1>());
                  }
                  return CResult::GenerateSuccess(result);
               }

               return CResult::GenerateError("invalid parameter. Can not retrieve keyword id in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving keyword");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CDevice::getAllKeywords(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               const auto result = m_keywordManager->getAllKeywords();
               shared::CDataContainer collection;
               collection.set("keywords", result);
               return CResult::GenerateSuccess(collection);
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving keyword");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CDevice::getDevicesWithCapacity(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 3)
               {
                  const shared::plugin::yPluginApi::EKeywordAccessMode cam(parameters[2]);
                  //read the capacity name
                  const auto capacityName = parameters[3];

                  //run query
                  const auto result = m_deviceRequester->getDeviceWithCapacity(capacityName, cam);
                  shared::CDataContainer collection;
                  collection.set(getRestKeyword(), result);
                  return CResult::GenerateSuccess(collection);
               }
               return CResult::GenerateError("invalid parameter. Can not retrieve capacity in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving device with get capacity");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CDevice::getDeviceWithCapacityType(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 3)
               {
                  const shared::plugin::yPluginApi::EKeywordAccessMode cam(parameters[2]);
                  const shared::plugin::yPluginApi::EKeywordDataType typ(parameters[3]);

                  //run query
                  const auto result = m_deviceRequester->getDeviceWithCapacityType(cam, typ);
                  shared::CDataContainer collection;
                  collection.set(getRestKeyword(), result);
                  return CResult::GenerateSuccess(collection);
               }
               return CResult::GenerateError("invalid parameter. Can not retrieve capacity in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving device with get capacity");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CDevice::getDeviceWithKeywordAccessMode(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 2)
               {
                  const shared::plugin::yPluginApi::EKeywordAccessMode cam(parameters[2]);

                  //run query
                  const auto result = m_deviceRequester->getDeviceWithKeywordAccessMode(cam);
                  shared::CDataContainer collection;
                  collection.set(getRestKeyword(), result);
                  return CResult::GenerateSuccess(collection);
               }
               return CResult::GenerateError("invalid parameter. Can not retrieve access-mode in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving device with get capacity");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CDevice::getDeviceWithKeywordHistoryDepth(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 2)
               {
                  const shared::plugin::yPluginApi::EHistoryDepth historyDepth(parameters[2]);

                  //run query
                  const auto result = m_deviceRequester->getDeviceWithKeywordHistoryDepth(historyDepth);
                  shared::CDataContainer collection;
                  collection.set(getRestKeyword(), result);
                  return CResult::GenerateSuccess(collection);
               }
               return CResult::GenerateError("invalid parameter. Can not retrieve history-depth in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving device with this history-depth policy");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CDevice::getDeviceMatchKeywordCriteria(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               // Get all devices matching some criteria on keywords.
               // Criteria are provided by a JSON string, like :
               //
               // {
               //    "expectedKeywordType": [
               //       "numeric",
               //       "bool",
               //       "Enum"
               //    ],
               //    "expectedCapacity" : ["curtain", "upDownStop"],
               //    "expectedKeywordAccess" : ["get"],
               //    "expectedKeywordHistoryDepth" : ["default"]
               // }
               //
               // The answer is a JSON string, like :
               // {
               //    "devices": [ deviceEntity1, deviceEntity2... ],
               //    "keywords" : [ keywordEntity1, keywordEntity2... ]
               // }
               //
               const auto criteria = shared::CDataContainer(requestContent);

               std::vector<shared::plugin::yPluginApi::EKeywordDataType> expectedKeywordTypes;
               if (criteria.exists("expectedKeywordType"))
               {
                  for (const auto& node : criteria.get<std::vector<std::string>>("expectedKeywordType"))
                     expectedKeywordTypes.emplace_back(node);
               }

               const auto expectedCapacities = criteria.exists("expectedCapacity")
                                                  ? criteria.get<std::vector<std::string>>("expectedCapacity")
                                                  : std::vector<std::string>();

               std::vector<shared::plugin::yPluginApi::EKeywordAccessMode> expectedKeywordAccesses;
               if (criteria.exists("expectedKeywordAccess"))
               {
                  for (const auto& node : criteria.get<std::vector<std::string>>("expectedKeywordAccess"))
                     expectedKeywordAccesses.emplace_back(node);
               }

               std::vector<shared::plugin::yPluginApi::EHistoryDepth> expectedKeywordHistoryDepth;
               if (criteria.exists("expectedKeywordHistoryDepth"))
               {
                  for (const auto& node : criteria.get<std::vector<std::string>>("expectedKeywordHistoryDepth"))
                     expectedKeywordHistoryDepth.emplace_back(node);
               }

               const auto& keywords = m_keywordManager->getKeywordsMatchingCriteria(expectedKeywordTypes,
                                                                                    expectedCapacities,
                                                                                    expectedKeywordAccesses,
                                                                                    expectedKeywordHistoryDepth,
                                                                                    false);
               std::set<int> deviceIds;
               for (const auto& keyword : keywords)
                  deviceIds.insert(keyword->DeviceId);
               const auto& devices = m_deviceRequester->getDevices(deviceIds);

               shared::CDataContainer collection;
               collection.set("devices", devices);
               collection.set("keywords", keywords);
               return CResult::GenerateSuccess(collection);
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving device with this history-depth policy");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CDevice::getDeviceMatchDeviceCriteria(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               // Get all devices matching some criteria.
               // Criteria are provided by a JSON string, like :
               //
               // {
               //    "expectedKeywordType": [
               //       "numeric",
               //       "bool",
               //       "Enum"
               //    ],
               //    "expectedName" : ["Forecast weather day + 1"]
               // }
               //
               // The answer is a JSON string, like :
               // {
               //    "devices": [ deviceEntity1, deviceEntity2... ]
               // }
               //
               const auto criteria = shared::CDataContainer(requestContent);

               const auto expectedNames = criteria.exists("expectedName")
                                             ? criteria.get<std::vector<std::string>>("expectedName")
                                             : std::vector<std::string>();

               const auto& devices = m_deviceManager->getDevicesMatchingCriteria(expectedNames,
                                                                                 false);

               shared::CDataContainer collection;
               collection.set("devices", devices);
               return CResult::GenerateSuccess(collection);
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving device with this history-depth policy");
            }
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CDevice::getDeviceKeywordsForCapacity(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 3)
               {
                  const auto deviceId = boost::lexical_cast<int>(parameters[1]);
                  const shared::plugin::yPluginApi::EKeywordAccessMode cam(parameters[2]);
                  const auto capacityName = parameters[3];


                  const auto result = m_keywordManager->getDeviceKeywordsWithCapacity(deviceId, capacityName, cam);
                  shared::CDataContainer collection;
                  collection.set("keyword", result);
                  return CResult::GenerateSuccess(collection);
               }
               return CResult::GenerateError("invalid parameter. Can not retrieve capacity in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving device with get capacity");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CDevice::getDeviceKeywords(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  const auto deviceId = boost::lexical_cast<int>(parameters[1]);
                  const auto deviceInDatabase = m_deviceRequester->getDevice(deviceId, true);

                  if (deviceInDatabase)
                  {
                     const auto allKeywordsForDevice = m_keywordManager->getKeywords(deviceId);
                     shared::CDataContainer collection;
                     collection.set("keyword", allKeywordsForDevice);
                     return CResult::GenerateSuccess(collection);
                  }
                  return CResult::GenerateError("Fail to retrieve device in database");
               }
               return CResult::GenerateError("invalid parameter. Can not retrieve capacity in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retrieving device with get capacity");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CDevice::sendKeywordCommand(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 2)
               {
                  //get keyword id from URL
                  const auto keywordId = boost::lexical_cast<int>(parameters[2]);

                  try
                  {
                     m_messageSender.sendKeywordCommandAsync(keywordId, requestContent);
                     return CResult::GenerateSuccess();
                  }
                  catch (shared::exception::CEmptyResult&)
                  {
                     return CResult::GenerateError("invalid parameter. Can not retrieve keyword in database");
                  }
               }
               return CResult::GenerateError("invalid parameter. Not enough parameters in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in sending command to keyword");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CDevice::deleteDevice(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  //get device id from URL
                  const auto deviceId = boost::lexical_cast<int>(parameters[1]);

                  const auto removeDevice = parameters.size() > 2 && parameters[2] == "removeDevice";

                  if (removeDevice)
                  {
                     m_pluginManager->notifyDeviceRemoved(deviceId);
                     m_deviceManager->removeDevice(deviceId);
                  }
                  else // Only blacklisted
                  {
                     m_deviceManager->cleanupDevice(deviceId);
                     m_deviceManager->updateDeviceBlacklistState(deviceId, true);
                  }

                  return CResult::GenerateSuccess();
               }
               return CResult::GenerateError("invalid parameter. Can not retrieve device id in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in reading device data");
            }
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CDevice::updateDeviceFriendlyName(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() >= 1)
               {
                  //get device id from URL
                  const auto deviceId = boost::lexical_cast<int>(parameters[1]);

                  database::entities::CDevice deviceToUpdate;
                  deviceToUpdate.fillFromSerializedString(requestContent);
                  if (deviceToUpdate.FriendlyName.isDefined())
                  {
                     //update data in base
                     m_deviceRequester->updateDeviceFriendlyName(deviceId, deviceToUpdate.FriendlyName());

                     //return the device info
                     const auto deviceFound = m_deviceRequester->getDevice(deviceId, true);
                     return CResult::GenerateSuccess(deviceFound);
                  }
                  return CResult::GenerateError("invalid request content. could not retrieve device friendlyName");
               }
               return CResult::GenerateError("invalid parameter. Can not retrieve device id in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in updating device friendly name");
            }
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CDevice::updateDeviceConfiguration(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() >= 1)
               {
                  //get device id from URL
                  const auto deviceId = boost::lexical_cast<int>(parameters[1]);

                  //deserialize device from request data
                  database::entities::CDevice deviceToUpdate;
                  deviceToUpdate.fillFromSerializedString(requestContent);

                  //update friendly name
                  if (deviceToUpdate.FriendlyName.isDefined())
                  {
                     m_deviceRequester->updateDeviceFriendlyName(deviceId, deviceToUpdate.FriendlyName());
                  }

                  if (deviceToUpdate.Model.isDefined())
                  {
                     m_deviceRequester->updateDeviceModel(deviceId, deviceToUpdate.Model());
                  }

                  if (deviceToUpdate.Configuration.isDefined())
                  {
                     //update data in base
                     m_deviceRequester->updateDeviceConfiguration(deviceId, deviceToUpdate.Configuration());
                     m_messageSender.sendSetDeviceConfiguration(deviceId,
                                                                deviceToUpdate.Configuration());
                  }

                  //return the device info
                  const auto deviceFound = m_deviceRequester->getDevice(deviceId, true);
                  return CResult::GenerateSuccess(deviceFound);
               }
               return CResult::GenerateError("invalid parameter. Can not retrieve device id in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in updating device configuration");
            }
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CDevice::mergeDevices(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               const shared::CDataContainer content(requestContent);
               const auto sourceDeviceId = content.get<int>("sourceDeviceId");
               const auto targetDeviceId = content.get<int>("targetDeviceId");
               const auto keywordCorrespondences = content.get<std::vector<shared::CDataContainer>>(
                  "keywordCorrespondences");

               // Merge acquisitions
               YADOMS_LOG(information) << "Starting merging 2 devices...";
               for (const auto& keywordCorrespondence : keywordCorrespondences)
               {
                  // Move acquisitions
                  const auto fromKw = keywordCorrespondence.get<int>("from");
                  const auto toKw = keywordCorrespondence.get<int>("to");
                  m_dataProvider->getAcquisitionRequester()->moveAllData(fromKw, toKw);
                  // Update last acquisition in keyword table
                  const auto lastData = m_dataProvider->getAcquisitionRequester()->getKeywordData(toKw,
                                                                                                  boost::posix_time::
                                                                                                  not_a_date_time,
                                                                                                  boost::posix_time::
                                                                                                  not_a_date_time,
                                                                                                  1);
                  m_dataProvider->getKeywordRequester()->updateLastValue(fromKw,
                                                                         lastData.size() != 0
                                                                            ? lastData[0].get<0>()
                                                                            : boost::posix_time::not_a_date_time,
                                                                         lastData.size() != 0
                                                                            ? lastData[0].get<1>()
                                                                            : std::string());
               }
               // Change name of target device, to make plugin using this device from now
               m_deviceRequester->rename(targetDeviceId, m_deviceRequester->getDevice(sourceDeviceId)->Name());

               // Remove source device
               m_deviceRequester->removeDevice(sourceDeviceId);

               YADOMS_LOG(information) << "Device merge done...";

               return CResult::GenerateSuccess();
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in merging devices");
            }
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CDevice::restoreDevice(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() >= 1)
               {
                  const auto deviceId = boost::lexical_cast<int>(parameters[1]);
                  m_deviceManager->updateDeviceBlacklistState(deviceId, false);

                  //return the device info
                  const auto deviceFound = m_deviceRequester->getDevice(deviceId, true);
                  return CResult::GenerateSuccess(deviceFound);
               }
               return CResult::GenerateError("invalid parameter. Can not retrieve device id in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in updating device configuration");
            }
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CDevice::updateKeywordFriendlyName(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 2)
               {
                  //get keyword id from URL
                  const auto keywordId = boost::lexical_cast<int>(parameters[2]);

                  database::entities::CKeyword keywordToUpdate;
                  keywordToUpdate.fillFromSerializedString(requestContent);
                  if (keywordToUpdate.FriendlyName.isDefined())
                  {
                     m_keywordManager->updateKeywordFriendlyName(keywordId, keywordToUpdate.FriendlyName());
                     return CResult::GenerateSuccess(m_keywordManager->getKeyword(keywordId));
                  }
                  return CResult::GenerateError("invalid request content. could not retrieve keyword friendlyName");
               }
               return CResult::GenerateError("invalid parameter. Can not retrieve device id in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in updating device friendly name");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CDevice::updateKeywordBlacklist(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 2)
               {
                  //get keyword id from URL
                  const auto keywordId = boost::lexical_cast<int>(parameters[2]);

                  database::entities::CKeyword keywordToUpdate;
                  keywordToUpdate.fillFromSerializedString(requestContent);
                  if (keywordToUpdate.Blacklist.isDefined())
                  {
                     m_keywordManager->updateKeywordBlacklistState(keywordId, keywordToUpdate.Blacklist());
                     return CResult::GenerateSuccess(m_keywordManager->getKeyword(keywordId));
                  }
                  return CResult::GenerateError("invalid request content. could not retrieve keyword blacklist");
               }
               return CResult::GenerateError("invalid parameter. Can not retrieve device id in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in updating device friendly name");
            }
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CDevice::transactionalMethod(
            CRestDispatcher::CRestMethodHandler realMethod,
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            auto pTransactionalEngine = m_dataProvider->getTransactionalEngine();
            boost::shared_ptr<shared::serialization::IDataSerializable> result;
            try
            {
               if (pTransactionalEngine)
                  pTransactionalEngine->transactionBegin();
               result = realMethod(parameters, requestContent);
            }
            catch (std::exception& ex)
            {
               result = CResult::GenerateError(ex);
            }
            catch (...)
            {
               result = CResult::GenerateError("unknown exception device rest method");
            }

            if (pTransactionalEngine)
            {
               if (CResult::isSuccess(*boost::dynamic_pointer_cast<shared::CDataContainer>(result)))
                  pTransactionalEngine->transactionCommit();
               else
                  pTransactionalEngine->transactionRollback();
            }
            return result;
         }
      } //namespace service
   } //namespace rest
} //namespace web 
