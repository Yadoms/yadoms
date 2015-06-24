#include "stdafx.h"
#include "GeneralRestService.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/json/JsonCollectionSerializer.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/RestDispatcher.h"
#include "web/rest/json/JsonResult.h"
#include <shared/Peripherals.h>

namespace web { namespace rest { namespace service {

   std::string CGeneralRestService::m_restKeyword= std::string("general");

   CGeneralRestService::CGeneralRestService()
   {

   }


   CGeneralRestService::~CGeneralRestService()
   {
   }


   void CGeneralRestService::configureDispatcher(CRestDispatcher & dispatcher)
   {
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("serialports"), CGeneralRestService::getSerialPorts);
   }


   const std::string & CGeneralRestService::getRestKeyword()
   {
      return m_restKeyword;
   }


   web::rest::json::CJson CGeneralRestService::getSerialPorts(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         web::rest::json::CJson result;
         const boost::shared_ptr<const shared::CPeripherals::SerialPortsMap> map = shared::CPeripherals::getSerialPorts();
         for(shared::CPeripherals::SerialPortsMap::const_iterator i = map->begin(); i != map->end(); ++i)
         {
            result.push_back(std::make_pair(i->first, web::rest::json::CJson(std::string(i->second))));
         }
         return web::rest::json::CJsonResult::GenerateSuccess(result);
      }
      catch(std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in retreiving all serial ports");
      }
   }
   

} //namespace service
} //namespace rest
} //namespace web 
