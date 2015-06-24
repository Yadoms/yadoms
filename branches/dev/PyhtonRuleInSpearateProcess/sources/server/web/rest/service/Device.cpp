#include "stdafx.h"
#include "Device.h"
#include <shared/exception/EmptyResult.hpp>
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/Result.h"
#include "web/rest/RestDispatcherHelpers.hpp"

namespace web { namespace rest { namespace service {

   std::string CDevice::m_restKeyword= std::string("device");


   CDevice::CDevice(boost::shared_ptr<database::IDataProvider> dataProvider, communication::ISendMessageAsync & messageSender)
      :m_dataProvider(dataProvider), m_messageSender(messageSender)
   {
   }


   CDevice::~CDevice()
   {
   }

   const std::string & CDevice::getRestKeyword()
   {
      return m_restKeyword;
   }

   void CDevice::configureDispatcher(CRestDispatcher & dispatcher)
   {
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CDevice::getAllDevices);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*"), CDevice::getOneDevice);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword"), CDevice::getAllKeywords);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("keyword")("*"), CDevice::getKeyword);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("matchcapacity")("*")("*"), CDevice::getDevicesWithCapacity);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("matchcapacitytype")("*")("*"), CDevice::getDeviceWithCapacityType);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*")("*")("*"), CDevice::getDeviceKeywordsForCapacity);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*")("keyword"), CDevice::getDeviceKeywords);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT",  (m_restKeyword)("*"), CDevice::updateDeviceFriendlyName, CDevice::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT",  (m_restKeyword)("keyword")("*"), CDevice::updateKeywordFriendlyName, CDevice::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST", (m_restKeyword)("keyword")("*")("command"), CDevice::sendDeviceCommand, CDevice::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE",  (m_restKeyword)("*"), CDevice::deleteDevice, CDevice::transactionalMethod);
   }

   shared::CDataContainer CDevice::getOneDevice(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::string objectId = "";
      if(parameters.size()>1)
         objectId = parameters[1];

      boost::shared_ptr<database::entities::CDevice> deviceFound =  m_dataProvider->getDeviceRequester()->getDevice(boost::lexical_cast<int>(objectId));
      return CResult::GenerateSuccess(deviceFound);
   }

   shared::CDataContainer CDevice::getAllDevices(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::vector< boost::shared_ptr<database::entities::CDevice> > dvList = m_dataProvider->getDeviceRequester()->getDevices();
      shared::CDataContainer collection;
      collection.set(getRestKeyword(), dvList);
      return CResult::GenerateSuccess(collection);
   }

   shared::CDataContainer CDevice::getKeyword(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if (parameters.size()>=2)
         {
            int keywordId = boost::lexical_cast<int>(parameters[2]);

            boost::shared_ptr<database::entities::CKeyword> keyword = m_dataProvider->getKeywordRequester()->getKeyword(keywordId);
            return CResult::GenerateSuccess(keyword);
         }
         return CResult::GenerateError("invalid parameter. Can not retreive keyword id in url");
      }
      catch (std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch (...)
      {
         return CResult::GenerateError("unknown exception in retreiving keyword");
      }
   }

   shared::CDataContainer CDevice::getAllKeywords(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         std::vector< boost::shared_ptr<database::entities::CKeyword> > result = m_dataProvider->getKeywordRequester()->getAllKeywords();
         shared::CDataContainer collection;
         collection.set("keywords", result);
         return CResult::GenerateSuccess(collection);
      }
      catch (std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch (...)
      {
         return CResult::GenerateError("unknown exception in retreiving keyword");
      }
   }

   shared::CDataContainer CDevice::getDevicesWithCapacity(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if(parameters.size()>2)
         {
            shared::plugin::yPluginApi::EKeywordAccessMode cam(parameters[2]);
            //read the capacity name
            std::string capacityName = parameters[3];

            //run query
            std::vector< boost::shared_ptr<database::entities::CDevice> > result = m_dataProvider->getDeviceRequester()->getDeviceWithCapacity(capacityName, cam);
            shared::CDataContainer collection;
            collection.set(getRestKeyword(), result);
            return CResult::GenerateSuccess(collection);
         }
         return CResult::GenerateError("invalid parameter. Can not retreive capacity in url");
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in retreiving device with get capacity");
      }
   }

   shared::CDataContainer CDevice::getDeviceWithCapacityType(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if(parameters.size()>2)
         {
            shared::plugin::yPluginApi::EKeywordAccessMode cam(parameters[2]);
            shared::plugin::yPluginApi::EKeywordDataType typ = parameters[3];

            //run query
            std::vector< boost::shared_ptr<database::entities::CDevice> > result = m_dataProvider->getDeviceRequester()->getDeviceWithCapacityType(cam, typ);
            shared::CDataContainer collection;
            collection.set(getRestKeyword(), result);
            return CResult::GenerateSuccess(collection);
         }
         return CResult::GenerateError("invalid parameter. Can not retreive capacity in url");
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in retreiving device with get capacity");
      }
   }

   shared::CDataContainer CDevice::getDeviceKeywordsForCapacity(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if(parameters.size()>3)
         {
            int deviceId = boost::lexical_cast<int>(parameters[1]);
            shared::plugin::yPluginApi::EKeywordAccessMode cam(parameters[2]);
            std::string capacityName = parameters[3];


            std::vector< boost::shared_ptr<database::entities::CKeyword> > result = m_dataProvider->getKeywordRequester()->getDeviceKeywordsWithCapacity(deviceId, capacityName, cam);
            shared::CDataContainer collection;
            collection.set("keyword", result);
            return CResult::GenerateSuccess(collection);
         }
         return CResult::GenerateError("invalid parameter. Can not retreive capacity in url");
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in retreiving device with get capacity");
      }
   }

   shared::CDataContainer CDevice::getDeviceKeywords(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if(parameters.size()>1)
         {
            int deviceId = boost::lexical_cast<int>(parameters[1]);
            boost::shared_ptr<database::entities::CDevice> deviceInDatabase = m_dataProvider->getDeviceRequester()->getDevice(deviceId);

            if(deviceInDatabase)
            {
               std::vector<boost::shared_ptr<database::entities::CKeyword> > allKeywordsforDevice = m_dataProvider->getKeywordRequester()->getKeywords(deviceId);
               shared::CDataContainer collection;
               collection.set("keyword", allKeywordsforDevice);
               return CResult::GenerateSuccess(collection);
            }
            return CResult::GenerateError("Fail to retrieve device in database");
         }
         return CResult::GenerateError("invalid parameter. Can not retreive capacity in url");
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in retreiving device with get capacity");
      }
   }

   shared::CDataContainer CDevice::sendDeviceCommand(const std::vector<std::string> & parameters, const std::string & requestContent)
   {
      try
      {
         if(parameters.size()>3)
         {
            //get keyword id from URL
            int keywordId = boost::lexical_cast<int>(parameters[2]);

            try
            {
               m_messageSender.sendCommandAsync(keywordId, requestContent);
               return CResult::GenerateSuccess();
            }
            catch (shared::exception::CEmptyResult&)
            {
               return CResult::GenerateError("invalid parameter. Can not retreive keyword in database");
            }
         }
         return CResult::GenerateError("invalid parameter. Not enough parameters in url");
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in sending command to device");
      }
   }




   shared::CDataContainer CDevice::deleteDevice(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if(parameters.size()>=1)
         {
            //get device id from URL
            int deviceId = boost::lexical_cast<int>(parameters[1]);

            //remove device in db
            m_dataProvider->getDeviceRequester()->removeDevice(deviceId);
            return CResult::GenerateSuccess();
         }
         return CResult::GenerateError("invalid parameter. Can not retreive device id in url");
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in reading device data");
      }
   }



   shared::CDataContainer CDevice::updateDeviceFriendlyName(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if(parameters.size()>=1)
         {
            //get device id from URL
            int deviceId = boost::lexical_cast<int>(parameters[1]);

            database::entities::CDevice deviceToUpdate;
            deviceToUpdate.fillFromContent(requestContent);
            if(deviceToUpdate.FriendlyName.isDefined())
            {
               //update data in base
               m_dataProvider->getDeviceRequester()->updateDeviceFriendlyName(deviceId, deviceToUpdate.FriendlyName());

               //return the device info
               boost::shared_ptr<database::entities::CDevice> deviceFound = m_dataProvider->getDeviceRequester()->getDevice(deviceId);
               return CResult::GenerateSuccess(deviceFound);

            }
            return CResult::GenerateError("invalid request content. could not retreive device friendlyName");
         }
         return CResult::GenerateError("invalid parameter. Can not retreive device id in url");
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in updating device friendly name");
      }
   }

   shared::CDataContainer CDevice::updateKeywordFriendlyName(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if(parameters.size()>=3)
         {
            //get keyword id from URL
            int keywordId = boost::lexical_cast<int>(parameters[2]);

            database::entities::CKeyword keywordToUpdate;
            keywordToUpdate.fillFromContent(requestContent);
            if(keywordToUpdate.FriendlyName.isDefined())
            {
               m_dataProvider->getKeywordRequester()->updateKeywordFriendlyName(keywordId, keywordToUpdate.FriendlyName());
               return CResult::GenerateSuccess(m_dataProvider->getKeywordRequester()->getKeyword(keywordId));
            }
            return CResult::GenerateError("invalid request content. could not retreive keyword friendlyName");
         }
         return CResult::GenerateError("invalid parameter. Can not retreive device id in url");
      }
      catch(std::exception &ex)
      {
         return CResult::GenerateError(ex);
      }
      catch(...)
      {
         return CResult::GenerateError("unknown exception in updating device friendly name");
      }
   }




   shared::CDataContainer CDevice::transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const std::string & requestContent)
   {
      boost::shared_ptr<database::ITransactionalProvider> pTransactionalEngine = m_dataProvider->getTransactionalEngine();
      shared::CDataContainer result;
      try
      {
         if(pTransactionalEngine)
            pTransactionalEngine->transactionBegin();
         result = realMethod(parameters, requestContent);
      }
      catch(std::exception &ex)
      {
         result = CResult::GenerateError(ex);
      }
      catch(...)
      {
         result = CResult::GenerateError("unknown exception device rest method");
      }

      if(pTransactionalEngine)
      {
         if(CResult::isSuccess(result))
            pTransactionalEngine->transactionCommit();
         else
            pTransactionalEngine->transactionRollback();
      }
      return result;
   }
} //namespace service
} //namespace rest
} //namespace web 
