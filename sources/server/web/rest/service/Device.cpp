#include "stdafx.h"
#include "Device.h"
#include <shared/exception/EmptyResult.hpp>
#include "web/rest/Result.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "communication/callback/SynchronousCallback.h"

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
              m_pluginManager(pluginManager),
              m_deviceManager(deviceManager),
              m_keywordManager(keywordManager),
              m_messageSender(messageSender)
         {
         }


         CDevice::~CDevice()
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
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("configurationSchema"), CDevice::getDeviceConfigurationSchema);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword"), CDevice::getAllKeywords);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*"), CDevice::getKeyword);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("matchcapacity")("*")("*"), CDevice::getDevicesWithCapacity);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("matchcapacitytype")("*")("*"), CDevice::getDeviceWithCapacityType);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("matchkeywordaccess")("*"), CDevice::getDeviceWithKeywordAccessMode);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("*")("*"), CDevice::getDeviceKeywordsForCapacity);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("*")("keyword"), CDevice::getDeviceKeywords);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*")("configuration"), CDevice::updateDeviceConfiguration, CDevice::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("*")("restore"), CDevice::restoreDevice, CDevice::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("keyword")("*"), CDevice::updateKeywordFriendlyName, CDevice::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT", (m_restKeyword)("keyword")("*")("blacklist"), CDevice::updateKeywordBlacklist, CDevice::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("keyword")("*")("command"), CDevice::sendKeywordCommand, CDevice::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("*")("command"), CDevice::sendDeviceCommand, CDevice::transactionalMethod);
            REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE", (m_restKeyword)("*")("*"), CDevice::deleteDevice, CDevice::transactionalMethod);
         }

         shared::CDataContainer CDevice::getOneDevice(const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            std::string objectId = "";
            if (parameters.size() > 1)
               objectId = parameters[1];

            auto deviceFound = m_dataProvider->getDeviceRequester()->getDevice(boost::lexical_cast<int>(objectId), true);
            return CResult::GenerateSuccess(deviceFound);
         }

         shared::CDataContainer CDevice::getDeviceConfigurationSchema(const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 2)
               {
                  auto deviceId = boost::lexical_cast<int>(parameters[1]);
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
                           auto res = cb.getCallbackResult();
                           if (res.Success)
                              return CResult::GenerateSuccess(res.Result);
                           return CResult::GenerateError(res.ErrorMessage);
                        }
                     case shared::event::kTimeout:
                        return CResult::GenerateError("The plugin did not respond");
                     default:
                        return CResult::GenerateError("Unkown plugin result");
                     }
                  }
                  catch (shared::exception::CException& ex)
                  {
                     return CResult::GenerateError(ex);
                  }
               }
               return CResult::GenerateError("invalid parameter. Can not retreive device id in url");
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

         shared::CDataContainer CDevice::getAllDevices(const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            auto dvList = m_dataProvider->getDeviceRequester()->getDevices(true);
            shared::CDataContainer collection;
            collection.set(getRestKeyword(), dvList);
            return CResult::GenerateSuccess(collection);
         }

         shared::CDataContainer CDevice::getKeyword(const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() >= 2)
               {
                  auto keywordId = boost::lexical_cast<int>(parameters[2]);

                  auto keyword = m_keywordManager->getKeyword(keywordId);
                  return CResult::GenerateSuccess(keyword);
               }
               return CResult::GenerateError("invalid parameter. Can not retreive keyword id in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retreiving keyword");
            }
         }

         shared::CDataContainer CDevice::getAllKeywords(const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               auto result = m_keywordManager->getAllKeywords();
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
               return CResult::GenerateError("unknown exception in retreiving keyword");
            }
         }

         shared::CDataContainer CDevice::getDevicesWithCapacity(const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 3)
               {
                  shared::plugin::yPluginApi::EKeywordAccessMode cam(parameters[2]);
                  //read the capacity name
                  auto capacityName = parameters[3];

                  //run query
                  auto result = m_dataProvider->getDeviceRequester()->getDeviceWithCapacity(capacityName, cam);
                  shared::CDataContainer collection;
                  collection.set(getRestKeyword(), result);
                  return CResult::GenerateSuccess(collection);
               }
               return CResult::GenerateError("invalid parameter. Can not retreive capacity in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retreiving device with get capacity");
            }
         }

         shared::CDataContainer CDevice::getDeviceWithCapacityType(const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 3)
               {
                  shared::plugin::yPluginApi::EKeywordAccessMode cam(parameters[2]);
                  shared::plugin::yPluginApi::EKeywordDataType typ(parameters[3]);

                  //run query
                  auto result = m_dataProvider->getDeviceRequester()->getDeviceWithCapacityType(cam, typ);
                  shared::CDataContainer collection;
                  collection.set(getRestKeyword(), result);
                  return CResult::GenerateSuccess(collection);
               }
               return CResult::GenerateError("invalid parameter. Can not retreive capacity in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retreiving device with get capacity");
            }
         }

         shared::CDataContainer CDevice::getDeviceWithKeywordAccessMode(const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 2)
               {
                  shared::plugin::yPluginApi::EKeywordAccessMode cam(parameters[2]);

                  //run query
                  auto result = m_dataProvider->getDeviceRequester()->getDeviceWithKeywordAccessMode(cam);
                  shared::CDataContainer collection;
                  collection.set(getRestKeyword(), result);
                  return CResult::GenerateSuccess(collection);
               }
               return CResult::GenerateError("invalid parameter. Can not retreive accessmode in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retreiving device with get capacity");
            }
         }

         

         shared::CDataContainer CDevice::getDeviceKeywordsForCapacity(const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 3)
               {
                  auto deviceId = boost::lexical_cast<int>(parameters[1]);
                  shared::plugin::yPluginApi::EKeywordAccessMode cam(parameters[2]);
                  auto capacityName = parameters[3];


                  auto result = m_keywordManager->getDeviceKeywordsWithCapacity(deviceId, capacityName, cam);
                  shared::CDataContainer collection;
                  collection.set("keyword", result);
                  return CResult::GenerateSuccess(collection);
               }
               return CResult::GenerateError("invalid parameter. Can not retreive capacity in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retreiving device with get capacity");
            }
         }

         shared::CDataContainer CDevice::getDeviceKeywords(const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  auto deviceId = boost::lexical_cast<int>(parameters[1]);
                  auto deviceInDatabase = m_dataProvider->getDeviceRequester()->getDevice(deviceId, true);

                  if (deviceInDatabase)
                  {
                     auto allKeywordsforDevice = m_keywordManager->getKeywords(deviceId);
                     shared::CDataContainer collection;
                     collection.set("keyword", allKeywordsforDevice);
                     return CResult::GenerateSuccess(collection);
                  }
                  return CResult::GenerateError("Fail to retrieve device in database");
               }
               return CResult::GenerateError("invalid parameter. Can not retreive capacity in url");
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retreiving device with get capacity");
            }
         }

         shared::CDataContainer CDevice::sendKeywordCommand(const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 2)
               {
                  //get keyword id from URL
                  auto keywordId = boost::lexical_cast<int>(parameters[2]);

                  try
                  {
                     m_messageSender.sendKeywordCommandAsync(keywordId, requestContent);
                     return CResult::GenerateSuccess();
                  }
                  catch (shared::exception::CEmptyResult&)
                  {
                     return CResult::GenerateError("invalid parameter. Can not retreive keyword in database");
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

         shared::CDataContainer CDevice::sendDeviceCommand(const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 2)
               {
                  //get device id from URL
                  auto deviceId = boost::lexical_cast<int>(parameters[1]);

                  try
                  {
                     m_messageSender.sendDeviceCommandAsync(deviceId, requestContent);
                     return CResult::GenerateSuccess();
                  }
                  catch (shared::exception::CEmptyResult&)
                  {
                     return CResult::GenerateError("invalid parameter. Can not retreive device in database");
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
               return CResult::GenerateError("unknown exception in sending command to device");
            }
         }

         shared::CDataContainer CDevice::deleteDevice(const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 1)
               {
                  //get device id from URL
                  auto deviceId = boost::lexical_cast<int>(parameters[1]);

                  auto removeDevice = parameters.size() > 2 && parameters[2] == "removeDevice";

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
               return CResult::GenerateError("invalid parameter. Can not retreive device id in url");
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


         shared::CDataContainer CDevice::updateDeviceFriendlyName(const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() >= 1)
               {
                  //get device id from URL
                  auto deviceId = boost::lexical_cast<int>(parameters[1]);

                  database::entities::CDevice deviceToUpdate;
                  deviceToUpdate.fillFromSerializedString(requestContent);
                  if (deviceToUpdate.FriendlyName.isDefined())
                  {
                     //update data in base
                     m_dataProvider->getDeviceRequester()->updateDeviceFriendlyName(deviceId, deviceToUpdate.FriendlyName());

                     //return the device info
                     auto deviceFound = m_dataProvider->getDeviceRequester()->getDevice(deviceId, true);
                     return CResult::GenerateSuccess(deviceFound);
                  }
                  return CResult::GenerateError("invalid request content. could not retreive device friendlyName");
               }
               return CResult::GenerateError("invalid parameter. Can not retreive device id in url");
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


         shared::CDataContainer CDevice::updateDeviceConfiguration(const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() >= 1)
               {
                  //get device id from URL
                  auto deviceId = boost::lexical_cast<int>(parameters[1]);

                  //deserialize device from request data
                  database::entities::CDevice deviceToUpdate;
                  deviceToUpdate.fillFromSerializedString(requestContent);

                  //update friendlyname
                  if (deviceToUpdate.FriendlyName.isDefined())
                  {
                     m_dataProvider->getDeviceRequester()->updateDeviceFriendlyName(deviceId, deviceToUpdate.FriendlyName());
                  }

                  if (deviceToUpdate.Model.isDefined())
                  {
                     m_dataProvider->getDeviceRequester()->updateDeviceModel(deviceId, deviceToUpdate.Model());
                  }

                  if (deviceToUpdate.Configuration.isDefined())
                  {
                     //update data in base
                     m_dataProvider->getDeviceRequester()->updateDeviceConfiguration(deviceId, deviceToUpdate.Configuration());
                     m_messageSender.sendSetDeviceConfiguration(deviceId,
                                                                deviceToUpdate.Configuration());
                  }

                  //return the device info
                  auto deviceFound = m_dataProvider->getDeviceRequester()->getDevice(deviceId, true);
                  return CResult::GenerateSuccess(deviceFound);
               }
               return CResult::GenerateError("invalid parameter. Can not retreive device id in url");
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


         shared::CDataContainer CDevice::restoreDevice(const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() >= 1)
               {
                  auto deviceId = boost::lexical_cast<int>(parameters[1]);
                  m_deviceManager->updateDeviceBlacklistState(deviceId, false);

                  //return the device info
                  auto deviceFound = m_dataProvider->getDeviceRequester()->getDevice(deviceId, true);
                  return CResult::GenerateSuccess(deviceFound);
               }
               return CResult::GenerateError("invalid parameter. Can not retreive device id in url");
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


         shared::CDataContainer CDevice::updateKeywordFriendlyName(const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 2)
               {
                  //get keyword id from URL
                  auto keywordId = boost::lexical_cast<int>(parameters[2]);

                  database::entities::CKeyword keywordToUpdate;
                  keywordToUpdate.fillFromSerializedString(requestContent);
                  if (keywordToUpdate.FriendlyName.isDefined())
                  {
                     m_keywordManager->updateKeywordFriendlyName(keywordId, keywordToUpdate.FriendlyName());
                     return CResult::GenerateSuccess(m_keywordManager->getKeyword(keywordId));
                  }
                  return CResult::GenerateError("invalid request content. could not retreive keyword friendlyName");
               }
               return CResult::GenerateError("invalid parameter. Can not retreive device id in url");
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

         shared::CDataContainer CDevice::updateKeywordBlacklist(const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            try
            {
               if (parameters.size() > 2)
               {
                  //get keyword id from URL
                  auto keywordId = boost::lexical_cast<int>(parameters[2]);

                  database::entities::CKeyword keywordToUpdate;
                  keywordToUpdate.fillFromSerializedString(requestContent);
                  if (keywordToUpdate.Blacklist.isDefined())
                  {
                     m_keywordManager->updateKeywordBlacklistState(keywordId, keywordToUpdate.Blacklist());
                     return CResult::GenerateSuccess(m_keywordManager->getKeyword(keywordId));
                  }
                  return CResult::GenerateError("invalid request content. could not retreive keyword blacklist");
               }
               return CResult::GenerateError("invalid parameter. Can not retreive device id in url");
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

         

         shared::CDataContainer CDevice::transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            auto pTransactionalEngine = m_dataProvider->getTransactionalEngine();
            shared::CDataContainer result;
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
               if (CResult::isSuccess(result))
                  pTransactionalEngine->transactionCommit();
               else
                  pTransactionalEngine->transactionRollback();
            }
            return result;
         }
      } //namespace service
   } //namespace rest
} //namespace web 


