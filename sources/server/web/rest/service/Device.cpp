#include "stdafx.h"
#include "Device.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/json/JsonSerializers.h"
#include "web/rest/json/JsonCollectionSerializer.h"
#include "web/rest/json/JsonResult.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/json/JsonGenericSerializer.h"
#include "web/rest/json/JsonDate.h"
#include "communication/command/DeviceCommand.h"

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
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("matchcapacity")("*")("*"), CDevice::getDevicesWithCapacity);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*")("*")("*"), CDevice::getDeviceKeywordsForCapacity);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*")("keyword"), CDevice::getDeviceKeywords);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT",  (m_restKeyword)("*"), CDevice::updateDeviceFriendlyName, CDevice::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "PUT",  (m_restKeyword)("keyword")("*"), CDevice::updateKeywordFriendlyName, CDevice::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST",  (m_restKeyword)("*")("command"), CDevice::sendDeviceCommand, CDevice::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST",  (m_restKeyword), CDevice::createDevice, CDevice::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE",  (m_restKeyword)("*"), CDevice::deleteDevice, CDevice::transactionalMethod);
   }

   web::rest::json::CJson CDevice::getOneDevice(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      std::string objectId = "";
      if(parameters.size()>1)
         objectId = parameters[1];

      web::rest::json::CDeviceEntitySerializer hes;
      boost::shared_ptr<database::entities::CDevice> deviceFound =  m_dataProvider->getDeviceRequester()->getDevice(boost::lexical_cast<int>(objectId));
      return web::rest::json::CJsonResult::GenerateSuccess(hes.serialize(*deviceFound.get()));
   }

   web::rest::json::CJson CDevice::getAllDevices(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      web::rest::json::CDeviceEntitySerializer hes;
      std::vector< boost::shared_ptr<database::entities::CDevice> > dvList = m_dataProvider->getDeviceRequester()->getDevices();
      return web::rest::json::CJsonResult::GenerateSuccess(web::rest::json::CJsonCollectionSerializer<database::entities::CDevice>::SerializeCollection(dvList, hes, getRestKeyword()));
   }






   web::rest::json::CJson CDevice::getDevicesWithCapacity(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         web::rest::json::CWidgetEntitySerializer hes;
         if(parameters.size()>2)
         {
            database::entities::EKeywordAccessMode cam = database::entities::kNoAccess;
            //read the capacity mode
            if(parameters[2] == "get")
               cam = database::entities::kRead;
            else if(parameters[2] == "set")
               cam = database::entities::kWrite;
            else
               return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive capacity access mode in url");

            //read the capacity name
            std::string capacityName = parameters[3];

            //run query
            web::rest::json::CDeviceEntitySerializer hes;
            std::vector< boost::shared_ptr<database::entities::CDevice> > result = m_dataProvider->getDeviceRequester()->getDeviceWithCapacity(capacityName, cam);
            return web::rest::json::CJsonResult::GenerateSuccess(web::rest::json::CJsonCollectionSerializer<database::entities::CDevice>::SerializeCollection(result, hes, getRestKeyword()));
         }
         else
         {
            return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive capacity in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in retreiving device with get capacity");
      }
   }





   web::rest::json::CJson CDevice::getDeviceKeywordsForCapacity(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         web::rest::json::CWidgetEntitySerializer hes;
         if(parameters.size()>3)
         {

            int deviceId = boost::lexical_cast<int>(parameters[1]);

            database::entities::EKeywordAccessMode cam = database::entities::kNoAccess;
            //read the capacity mode
            if(parameters[2] == "get")
               cam = database::entities::kRead;
            else if(parameters[2] == "set")
               cam = database::entities::kWrite;
            else
               return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive capacity access mode in url");

            //read the capacity name
            std::string capacityName = parameters[3];


            web::rest::json::CKeywordEntitySerializer hes;
            std::vector< boost::shared_ptr<database::entities::CKeyword> > result = m_dataProvider->getKeywordRequester()->getDeviceKeywordsWithCapacity(deviceId, capacityName, cam);
            return web::rest::json::CJsonResult::GenerateSuccess(web::rest::json::CJsonCollectionSerializer<database::entities::CKeyword>::SerializeCollection(result, hes, "keyword"));
         }
         else
         {
            return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive capacity in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in retreiving device with get capacity");
      }
   }



   web::rest::json::CJson CDevice::getDeviceKeywords(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         web::rest::json::CWidgetEntitySerializer hes;
         if(parameters.size()>1)
         {
            int deviceId = boost::lexical_cast<int>(parameters[1]);
            boost::shared_ptr<database::entities::CDevice> deviceInDatabase = m_dataProvider->getDeviceRequester()->getDevice(deviceId);

            if(deviceInDatabase)
            {
               std::vector<boost::shared_ptr<database::entities::CKeyword> > allKeywordsforDevice = m_dataProvider->getKeywordRequester()->getKeywords(deviceId);
               web::rest::json::CKeywordEntitySerializer hes;
               return web::rest::json::CJsonResult::GenerateSuccess(web::rest::json::CJsonCollectionSerializer<database::entities::CKeyword>::SerializeCollection(allKeywordsforDevice, hes, "keyword"));
            }
            else
            {
               return web::rest::json::CJsonResult::GenerateError("Fail to retrieve device in database");
            }
         }
         else
         {
            return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive capacity in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in retreiving device with get capacity");
      }
   }



   web::rest::json::CJson CDevice::sendDeviceCommand(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         if(parameters.size()>1)
         {
            //get device id from URL
            int deviceId = boost::lexical_cast<int>(parameters[1]);

            //create the command data (all request content values are stored into a single map
            communication::command::CDeviceCommand::CommandData commandData;
            BOOST_FOREACH(const web::rest::json::CJson::value_type &v, requestContent)
            {
               commandData.insert(std::make_pair(v.first.data(), v.second.data()));
            }

            //create the command
            boost::shared_ptr<communication::command::CCallback> resultHandler(new communication::command::CCallback);
            communication::command::CDeviceCommand command(deviceId, commandData, resultHandler);

            //send the command
            m_messageSender.sendCommandAsync(command);

            //wait for a result
            communication::command::CResult result = resultHandler->waitForResult(boost::posix_time::milliseconds(2000));

            //reply to rest caller
            if(result.isSuccess())
               return web::rest::json::CJsonResult::GenerateSuccess();
            else
               return web::rest::json::CJsonResult::GenerateError(result.getErrorMessage());
         }
         else
         {
            return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not device widget id in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in sending command to device");
      }
   }


   web::rest::json::CJson CDevice::createDevice(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         web::rest::json::CDeviceEntitySerializer des;
         boost::shared_ptr<database::entities::CDevice> deviceToAdd = des.deserialize(requestContent);
         boost::shared_ptr<database::entities::CDevice> deviceFound = m_dataProvider->getDeviceRequester()->createDevice(deviceToAdd->PluginId(), deviceToAdd->Name(), deviceToAdd->FriendlyName(), deviceToAdd->Model());
         return web::rest::json::CJsonResult::GenerateSuccess(des.serialize(*deviceFound.get()));
      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in creating a device");
      }
   }



   web::rest::json::CJson CDevice::deleteDevice(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {

         if(parameters.size()>=1)
         {
            //get device id from URL
            int deviceId = boost::lexical_cast<int>(parameters[1]);

            //remove device in db
            m_dataProvider->getDeviceRequester()->removeDevice(deviceId);
            return web::rest::json::CJsonResult::GenerateSuccess();
         }
         else
         {
            return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive device id in url");
         }

      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in reading device data");
      }
   }



   web::rest::json::CJson CDevice::updateDeviceFriendlyName(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {

         if(parameters.size()>=1)
         {
            //get device id from URL
            int deviceId = boost::lexical_cast<int>(parameters[1]);

            web::rest::json::CDeviceEntitySerializer des;
            boost::shared_ptr<database::entities::CDevice> deviceToUpdate = des.deserialize(requestContent);
            if(deviceToUpdate->FriendlyName.isDefined())
            {
               m_dataProvider->getDeviceRequester()->updateDeviceFriendlyName(deviceId, deviceToUpdate->FriendlyName());
               return web::rest::json::CJsonResult::GenerateSuccess();
            }
            else
            {
               return web::rest::json::CJsonResult::GenerateError("invalid request content. could not retreive device friendlyName");
            }
         }
         else
         {
            return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive device id in url");
         }

      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in updating device friendly name");
      }
   }

   web::rest::json::CJson CDevice::updateKeywordFriendlyName(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {

         if(parameters.size()>=3)
         {
            //get keyword id from URL
            int keywordId = boost::lexical_cast<int>(parameters[2]);

            web::rest::json::CKeywordEntitySerializer des;
            boost::shared_ptr<database::entities::CKeyword> keywordToUpdate = des.deserialize(requestContent);
            if(keywordToUpdate->FriendlyName.isDefined())
            {
               m_dataProvider->getKeywordRequester()->updateKeywordFriendlyName(keywordId, keywordToUpdate->FriendlyName());
               return web::rest::json::CJsonResult::GenerateSuccess();
            }
            else
            {
               return web::rest::json::CJsonResult::GenerateError("invalid request content. could not retreive keyword friendlyName");
            }
         }
         else
         {
            return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive device id in url");
         }

      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in updating device friendly name");
      }
   }




   web::rest::json::CJson CDevice::transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      boost::shared_ptr<database::ITransactionalProvider> pTransactionalEngine = m_dataProvider->getTransactionalEngine();
      web::rest::json::CJson result;
      try
      {
         if(pTransactionalEngine)
            pTransactionalEngine->transactionBegin();
         result = realMethod(parameters, requestContent);
      }
      catch(std::exception &ex)
      {
         result = web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         result = web::rest::json::CJsonResult::GenerateError("unknown exception device rest method");
      }

      if(pTransactionalEngine)
      {
         if(web::rest::json::CJsonResult::isSuccess(result))
            pTransactionalEngine->transactionCommit();
         else
            pTransactionalEngine->transactionRollback();
      }
      return result;
   }
} //namespace service
} //namespace rest
} //namespace web 
