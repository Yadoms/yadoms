#include "stdafx.h"
#include "DeviceRestService.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/json/JsonSerializers.h"
#include "web/rest/json/JsonCollectionSerializer.h"
#include "web/rest/json/JsonResult.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/json/JsonGenericSerializer.h"
#include "web/rest/json/JsonDate.h"


namespace web { namespace rest { namespace service {

   std::string CDeviceRestService::m_restKeyword= std::string("device");


   CDeviceRestService::CDeviceRestService(boost::shared_ptr<database::IDataProvider> dataProvider, shared::event::CEventHandler & eventHandler)
      :m_dataProvider(dataProvider), m_eventHandler(eventHandler)
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
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET",  (m_restKeyword)("*")("lastdata"), CDeviceRestService::getLastDeviceData);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "POST",  (m_restKeyword)("*")("command"), CDeviceRestService::sendDeviceCommand);
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

            for(i=allData.begin(); i!=allData.end(); i++)
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
            return web::rest::json::CJsonResult::GenerateError("invalid parameter. Can not retreive widget id in utrl");
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
         
         return web::rest::json::CJsonResult::GenerateSuccess();
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



   /*
   web::rest::json::CJson CDeviceRestService::getDeviceLastAcquisition(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
   std::string objectId = "";
   if(parameters.size()>1)
   objectId = parameters[1];

   boost::shared_ptr<database::entities::CDevice> deviceFound =  m_dataProvider->getDeviceRequester()->getDevice(boost::lexical_cast<int>(objectId));
   if(deviceFound.get() != NULL)
   {
   web::rest::json::CAcquisitionEntitySerializer hes;
   std::vector< boost::shared_ptr<database::entities::CAcquisition> > allAcq =  m_dataProvider->getAcquisitionRequester()->getLastAcquisitions(deviceFound->getDataSource());
   return web::rest::json::CJsonResult::GenerateSuccess(web::rest::json::CJsonCollectionSerializer<database::entities::CAcquisition>::SerializeCollection(allAcq, hes, CAcquisitionRestService::getRestKeyword()));
   }
   else
   return web::rest::json::CJsonResult::GenerateError("Device not found");
   }
   */


} //namespace service
} //namespace rest
} //namespace web 
