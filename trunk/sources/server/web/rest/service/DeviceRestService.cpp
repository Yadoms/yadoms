#include "stdafx.h"
#include "DeviceRestService.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/json/JsonSerializers.h"
#include "web/rest/json/JsonCollectionSerializer.h"
#include "web/rest/json/JsonResult.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/json/JsonGenericSerializer.h"
#include "web/rest/json/JsonDate.h"
#include "communication/command/DeviceCommand.h"

namespace web { namespace rest { namespace service {

   std::string CDeviceRestService::m_restKeyword= std::string("device");


   CDeviceRestService::CDeviceRestService(boost::shared_ptr<database::IDataProvider> dataProvider, communication::ISendMessageAsync & messageSender)
      :m_dataProvider(dataProvider), m_messageSender(messageSender)
   {
   }


   CDeviceRestService::~CDeviceRestService()
   {
   }

   const std::string & CDeviceRestService::getRestKeyword()
   {
      return m_restKeyword;
   }

   void CDeviceRestService::configureDispatcher(CRestDispatcher & dispatcher)
   {
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword), CDeviceRestService::getAllDevices);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*"), CDeviceRestService::getOneDevice);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("matchkeyword")("*"), CDeviceRestService::getDeviceWithKeyword);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("matchprotocol")("*"), CDeviceRestService::getDeviceWhichSupportProtocol);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("matchprotocol")("*")("*"), CDeviceRestService::getDeviceWhichSupportProtocolAndKeyword);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*")("lastdata"), CDeviceRestService::getLastDeviceData);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*")("data")("*"), CDeviceRestService::getDeviceData); //get all keyword data
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*")("data")("*")("*"), CDeviceRestService::getDeviceData); //get keyword data from date
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*")("data")("*")("*")("*"), CDeviceRestService::getDeviceData); //get keyword data between two dates
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("hardwares"), CDeviceRestService::getDeviceHardwares);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("hardwares")("*")("protocols"), CDeviceRestService::getDeviceHardwareProtocols);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("generate")("*")("*"), CDeviceRestService::generateVirtualDevice);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST",  (m_restKeyword)("*")("command"), CDeviceRestService::sendDeviceCommand, CDeviceRestService::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "POST",  (m_restKeyword), CDeviceRestService::createDevice, CDeviceRestService::transactionalMethod);
      REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, "DELETE",  (m_restKeyword)("*"), CDeviceRestService::deleteDevice, CDeviceRestService::transactionalMethod);
   }

   web::rest::json::CJson CDeviceRestService::getOneDevice(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      std::string objectId = "";
      if(parameters.size()>1)
         objectId = parameters[1];

      web::rest::json::CDeviceEntitySerializer hes;
      boost::shared_ptr<database::entities::CDevice> deviceFound =  m_dataProvider->getDeviceRequester()->getDevice(boost::lexical_cast<int>(objectId));
      return web::rest::json::CJsonResult::GenerateSuccess(hes.serialize(*deviceFound.get()));
   }

   web::rest::json::CJson CDeviceRestService::getAllDevices(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      web::rest::json::CDeviceEntitySerializer hes;
      std::vector< boost::shared_ptr<database::entities::CDevice> > dvList = m_dataProvider->getDeviceRequester()->getDevices();
      return web::rest::json::CJsonResult::GenerateSuccess(web::rest::json::CJsonCollectionSerializer<database::entities::CDevice>::SerializeCollection(dvList, hes, getRestKeyword()));
   }

   web::rest::json::CJson CDeviceRestService::getDeviceWithKeyword(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      std::string keyword = "";
      if(parameters.size()>2)
      {
         keyword = parameters[2];
         web::rest::json::CDeviceEntitySerializer hes;
         std::vector< boost::shared_ptr<database::entities::CDevice> > dvList = m_dataProvider->getDeviceRequester()->getDevicesMatchingKeyword(keyword);
         return web::rest::json::CJsonResult::GenerateSuccess(web::rest::json::CJsonCollectionSerializer<database::entities::CDevice>::SerializeCollection(dvList, hes, getRestKeyword()));
      }
      else
      {
         return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive keyword in url");
      }
   }


   web::rest::json::CJson CDeviceRestService::getDeviceWhichSupportProtocol(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      std::string protocol = "";
      if(parameters.size()>2)
      {
         protocol = parameters[2];
         web::rest::json::CDeviceEntitySerializer hes;
         std::vector< boost::shared_ptr<database::entities::CDevice> > dvList = m_dataProvider->getDeviceRequester()->getDevicesMatchingProtocol(protocol);
         return web::rest::json::CJsonResult::GenerateSuccess(web::rest::json::CJsonCollectionSerializer<database::entities::CDevice>::SerializeCollection(dvList, hes, getRestKeyword()));
      }
      else
      {
         return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive protocol in url");
      }
   }


   web::rest::json::CJson CDeviceRestService::getDeviceWhichSupportProtocolAndKeyword(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      std::string protocol = "";
      std::string keyword = "";
      if(parameters.size()>3)
      {
         protocol = parameters[2];
         keyword = parameters[3];
         web::rest::json::CDeviceEntitySerializer hes;
         std::vector< boost::shared_ptr<database::entities::CDevice> > dvList = m_dataProvider->getDeviceRequester()->getDevicesMatchingProtocolWithKeyword(protocol, keyword);
         return web::rest::json::CJsonResult::GenerateSuccess(web::rest::json::CJsonCollectionSerializer<database::entities::CDevice>::SerializeCollection(dvList, hes, getRestKeyword()));
      }
      else
      {
         return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive protocol and keyword in url");
      }
   }



   web::rest::json::CJson CDeviceRestService::getLastDeviceData(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         web::rest::json::CWidgetEntitySerializer hes;
         if(parameters.size()>1)
         {
            int deviceId = 0;
            if(parameters.size()>1)
               deviceId = boost::lexical_cast<int>(parameters[1]);

            std::vector< boost::tuple<boost::posix_time::ptime, std::string, std::string>  > allData =  m_dataProvider->getDeviceRequester()->getDeviceLastData(deviceId);

            web::rest::json::CJson objectList;
            std::vector< boost::tuple<boost::posix_time::ptime, std::string, std::string> >::const_iterator i;

            for(i=allData.begin(); i!=allData.end(); ++i)
            {
               web::rest::json::CJson result;
               result.put("date", web::rest::json::CJsonDate::toString(i->get<0>()));
               result.put("key", i->get<1>());
               result.put("value", i->get<2>());

               objectList.push_back(std::make_pair("", result));
            }

            return web::rest::json::CJsonResult::GenerateSuccess(objectList);
         }
         else
         {
            return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive widget id in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in retreiving last device data");
      }
   }

   web::rest::json::CJson CDeviceRestService::sendDeviceCommand(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
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



   web::rest::json::CJson CDeviceRestService::getDeviceHardwares(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         std::vector<std::string> allHardwares = m_dataProvider->getDeviceRequester()->getDeviceHardwares();
         web::rest::json::CJson objectList = web::rest::json::CJsonCollectionSerializer<std::string>::SerializeCollection(allHardwares, "");
         return web::rest::json::CJsonResult::GenerateSuccess(objectList);
      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in getting available device hardwares");
      }
   }

   web::rest::json::CJson CDeviceRestService::getDeviceHardwareProtocols(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         if(parameters.size()>1)
         {
            //get hardware id from URL
            std::string hardwareId = parameters[2];

            std::vector<std::string> allProtocols =  m_deviceRules.getHardwareProtocols(hardwareId);
            web::rest::json::CJson objectList = web::rest::json::CJsonCollectionSerializer<std::string>::SerializeCollection(allProtocols, "");
            return web::rest::json::CJsonResult::GenerateSuccess(objectList);
         }
         else
         {
            return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive hardware identifier in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in getting hardware protocols");
      }
   }

   web::rest::json::CJson CDeviceRestService::generateVirtualDevice(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         if(parameters.size()>2)
         {
            //get hardware id from URL
            std::string hardwareId = parameters[2];
            std::string protocolId = parameters[3];

            web::rest::json::CJson result;
            result.put_value(m_deviceRules.generateVirtualDeviceIdentifier(hardwareId, protocolId));
            return web::rest::json::CJsonResult::GenerateSuccess(result);
         }
         else
         {
            return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive hardware identifier and protocol identifier in url");
         }
      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in generating a virtual device");
      }
   }

   web::rest::json::CJson CDeviceRestService::createDevice(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         web::rest::json::CDeviceEntitySerializer des;
         boost::shared_ptr<database::entities::CDevice> deviceToAdd = des.deserialize(requestContent);
         boost::shared_ptr<database::entities::CDevice> deviceFound = m_dataProvider->getDeviceRequester()->createDevice(deviceToAdd->Address(), deviceToAdd->Protocol(), deviceToAdd->HardwareIdentifier(), deviceToAdd->Name());
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


   web::rest::json::CJson CDeviceRestService::getDeviceData(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {

         if(parameters.size()>=4)
         {
            //get device id from URL
            int deviceId = boost::lexical_cast<int>(parameters[1]);
            boost::shared_ptr<database::entities::CDevice> deviceFound = m_dataProvider->getDeviceRequester()->getDevice(deviceId);
            if(deviceFound)
            {
               //get the keyword
               std::string keyword = parameters[3];
               boost::shared_ptr<database::entities::CKeyword> keywordFound = m_dataProvider->getKeywordRequester()->getKeyword(deviceId, keyword);
               if(keywordFound)
               {
                  //try to get from and to limits.
                  //as this method is common for three rest url, those variable may keep unfilled
                  boost::posix_time::ptime timeFrom, timeTo;

                  if(parameters.size()>=5)
                     timeFrom = web::rest::json::CJsonDate::fromString(parameters[4]);

                  if(parameters.size()>=6)
                     timeTo = web::rest::json::CJsonDate::fromString(parameters[5]);


                  std::vector< boost::tuple<boost::posix_time::ptime, std::string> > allData = m_dataProvider->getDeviceRequester()->getDeviceData(deviceId, keyword, timeFrom, timeTo);
                  web::rest::json::CJson objectList;
                  std::vector< boost::tuple<boost::posix_time::ptime, std::string> >::const_iterator i;

                  for(i=allData.begin(); i!=allData.end(); ++i)
                  {
                     web::rest::json::CJson result;
                     result.put("date", web::rest::json::CJsonDate::toString(i->get<0>()));
                     result.put("key", i->get<1>());
                     objectList.push_back(std::make_pair("", result));
                  }

                  web::rest::json::CJson result;


                  web::rest::json::CDeviceEntitySerializer des;
                  web::rest::json::CKeywordEntitySerializer kes;


                  result.put_child("device", des.serialize(*deviceFound.get()));
                  result.put_child("keyword", kes.serialize(*keywordFound.get()));
                  result.put_child("data", objectList);

                  return web::rest::json::CJsonResult::GenerateSuccess(result);
               }
               else
               {
                  return web::rest::json::CJsonResult::GenerateError("unknow keyword for this device");
               }
            }
            else
            {
               return web::rest::json::CJsonResult::GenerateError("can not retreive device");
            }
         }
         else
         {
            return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive parameters in url");
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


   web::rest::json::CJson CDeviceRestService::deleteDevice(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
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

   web::rest::json::CJson CDeviceRestService::transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
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
