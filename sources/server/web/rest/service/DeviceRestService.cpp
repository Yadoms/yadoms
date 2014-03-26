#include "stdafx.h"
#include "DeviceRestService.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/json/JsonSerializers.h"
#include "web/rest/json/JsonCollectionSerializer.h"
#include "web/rest/json/JsonResult.h"
#include "web/rest/RestDispatcherHelpers.hpp"

namespace web { namespace rest { namespace service {

   std::string CDeviceRestService::m_restKeyword= std::string("device");


   CDeviceRestService::CDeviceRestService(boost::shared_ptr<database::IDataProvider> dataProvider)
      :m_dataProvider(dataProvider)
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
