#include "stdafx.h"
#include "Device.h"
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
      return web::rest::CResult::GenerateSuccess(deviceFound);
   }

   shared::CDataContainer CDevice::getAllDevices(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      std::vector< boost::shared_ptr<database::entities::CDevice> > dvList = m_dataProvider->getDeviceRequester()->getDevices();
      shared::CDataContainer collection;
      collection.set(getRestKeyword(), dvList);
      return web::rest::CResult::GenerateSuccess(collection);
   }

   shared::CDataContainer CDevice::getKeyword(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if (parameters.size()>=2)
         {
            int keywordId = boost::lexical_cast<int>(parameters[2]);

            boost::shared_ptr<database::entities::CKeyword> keyword = m_dataProvider->getKeywordRequester()->getKeyword(keywordId);
            return web::rest::CResult::GenerateSuccess(keyword);
         }
         else
         {
            return web::rest::CResult::GenerateError("invalid parameter. Can not retreive keyword id in url");
         }
      }
      catch (std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch (...)
      {
         return web::rest::CResult::GenerateError("unknown exception in retreiving keyword");
      }
   }

   shared::CDataContainer CDevice::getDevicesWithCapacity(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if(parameters.size()>2)
         {
            shared::plugin::yadomsApi::EKeywordAccessMode cam(parameters[2]);
            //read the capacity name
            std::string capacityName = parameters[3];

            //run query
            std::vector< boost::shared_ptr<database::entities::CDevice> > result = m_dataProvider->getDeviceRequester()->getDeviceWithCapacity(capacityName, cam);
            shared::CDataContainer collection;
            collection.set(getRestKeyword(), result);
            return web::rest::CResult::GenerateSuccess(collection);
         }
         else
         {
            return web::rest::CResult::GenerateError("invalid parameter. Can not retreive capacity in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in retreiving device with get capacity");
      }
   }

   shared::CDataContainer CDevice::getDeviceWithCapacityType(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if(parameters.size()>2)
         {
            shared::plugin::yadomsApi::EKeywordAccessMode cam(parameters[2]);
            shared::plugin::yadomsApi::EKeywordDataType typ = parameters[3];

            //run query
            std::vector< boost::shared_ptr<database::entities::CDevice> > result = m_dataProvider->getDeviceRequester()->getDeviceWithCapacityType(cam, typ);
            shared::CDataContainer collection;
            collection.set(getRestKeyword(), result);
            return web::rest::CResult::GenerateSuccess(collection);
         }
         else
         {
            return web::rest::CResult::GenerateError("invalid parameter. Can not retreive capacity in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in retreiving device with get capacity");
      }
   }

   shared::CDataContainer CDevice::getDeviceKeywordsForCapacity(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if(parameters.size()>3)
         {
            int deviceId = boost::lexical_cast<int>(parameters[1]);
            shared::plugin::yadomsApi::EKeywordAccessMode cam(parameters[2]);
            std::string capacityName = parameters[3];


            std::vector< boost::shared_ptr<database::entities::CKeyword> > result = m_dataProvider->getKeywordRequester()->getDeviceKeywordsWithCapacity(deviceId, capacityName, cam);
            shared::CDataContainer collection;
            collection.set("keyword", result);
            return web::rest::CResult::GenerateSuccess(collection);
         }
         else
         {
            return web::rest::CResult::GenerateError("invalid parameter. Can not retreive capacity in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in retreiving device with get capacity");
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
               return web::rest::CResult::GenerateSuccess(collection);
            }
            else
            {
               return web::rest::CResult::GenerateError("Fail to retrieve device in database");
            }
         }
         else
         {
            return web::rest::CResult::GenerateError("invalid parameter. Can not retreive capacity in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in retreiving device with get capacity");
      }
   }

   shared::CDataContainer CDevice::sendDeviceCommand(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         if(parameters.size()>3)
         {
            //get keyword id from URL
            int keywordId = boost::lexical_cast<int>(parameters[2]);

            //retreive devideId from keywordId
            boost::shared_ptr<database::entities::CKeyword> keywordfromDb = m_dataProvider->getKeywordRequester()->getKeyword(keywordId);
            if (keywordfromDb)
            {
               int deviceId = m_dataProvider->getKeywordRequester()->getKeyword(keywordId)->DeviceId;

               //send the command
               m_messageSender.sendCommandAsync(deviceId, keywordId, requestContent);
               return web::rest::CResult::GenerateSuccess();
            }
            else
            {
               return web::rest::CResult::GenerateError("invalid parameter. Can not retreive keyword in database");
            }

         }
         else
         {
            return web::rest::CResult::GenerateError("invalid parameter. Not enough parameters in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in sending command to device");
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
            return web::rest::CResult::GenerateSuccess();
         }
         else
         {
            return web::rest::CResult::GenerateError("invalid parameter. Can not retreive device id in url");
         }

      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in reading device data");
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
               return web::rest::CResult::GenerateSuccess(deviceFound);

            }
            else
            {
               return web::rest::CResult::GenerateError("invalid request content. could not retreive device friendlyName");
            }
         }
         else
         {
            return web::rest::CResult::GenerateError("invalid parameter. Can not retreive device id in url");
         }

      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in updating device friendly name");
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
               return web::rest::CResult::GenerateSuccess(m_dataProvider->getKeywordRequester()->getKeyword(keywordId));
            }
            else
            {
               return web::rest::CResult::GenerateError("invalid request content. could not retreive keyword friendlyName");
            }
         }
         else
         {
            return web::rest::CResult::GenerateError("invalid parameter. Can not retreive device id in url");
         }

      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in updating device friendly name");
      }
   }




   shared::CDataContainer CDevice::transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
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
         result = web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         result = web::rest::CResult::GenerateError("unknown exception device rest method");
      }

      if(pTransactionalEngine)
      {
         if(web::rest::CResult::isSuccess(result))
            pTransactionalEngine->transactionCommit();
         else
            pTransactionalEngine->transactionRollback();
      }
      return result;
   }
} //namespace service
} //namespace rest
} //namespace web 
