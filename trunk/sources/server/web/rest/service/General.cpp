#include "stdafx.h"
#include "General.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/json/JsonCollectionSerializer.h"
#include "web/rest/json/JsonDate.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/RestDispatcher.h"
#include "web/rest/json/JsonResult.h"
#include <shared/Peripherals.h>

namespace web { namespace rest { namespace service {

   std::string CGeneral::m_restKeyword= std::string("general");

   CGeneral::CGeneral(boost::shared_ptr<CSystem> systemInformation)
      :m_systemInformation(systemInformation)
   {
      
   }


   CGeneral::~CGeneral()
   {
   }


   void CGeneral::configureDispatcher(CRestDispatcher & dispatcher)
   {
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("serialport"), CGeneral::getSerialPorts);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("system"), CGeneral::getSystemInformation);
   }


   const std::string & CGeneral::getRestKeyword()
   {
      return m_restKeyword;
   }


   web::rest::json::CJson CGeneral::getSerialPorts(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         web::rest::json::CJson result;
         const boost::shared_ptr<const shared::CPeripherals::SerialPortsMap> map = shared::CPeripherals::getSerialPorts();
         for(shared::CPeripherals::SerialPortsMap::const_iterator i = map->begin(); i != map->end(); ++i)
         {
            web::rest::json::CJson serialPort;
            serialPort.put("name", i->first);
            serialPort.put("friendlyName", i->second);
            result.push_back(std::make_pair("", serialPort));
         }

         web::rest::json::CJson result2;
         result2.push_back(std::make_pair("serialPorts", result));
         return web::rest::json::CJsonResult::GenerateSuccess(result2);
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
   
   web::rest::json::CJson CGeneral::getSystemInformation(const std::vector<std::string> & parameters, const web::rest::json::CJson & requestContent)
   {
      try
      {
         web::rest::json::CJson result;

         result.put("runningPlatform", m_systemInformation->getOperatingSystemName());
         result.put("yadomsVersion", m_systemInformation->getSoftwareVersion().toString());
         result.put("startupTime", web::rest::json::CJsonDate::toString(m_systemInformation->getStartupDateTime()));
         return web::rest::json::CJsonResult::GenerateSuccess(result);
      }
      catch (std::exception &ex)
      {
         return web::rest::json::CJsonResult::GenerateError(ex);
      }
      catch (...)
      {
         return web::rest::json::CJsonResult::GenerateError("unknown exception in retreiving system information");
      }
   }

} //namespace service
} //namespace rest
} //namespace web 
