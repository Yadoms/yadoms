#include "stdafx.h"
#include "System.h"
#include <shared/exception/NotImplemented.hpp>
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/RestDispatcher.h"
#include "web/rest/Result.h"
#include <shared/Peripherals.h>
#include <Poco/Net/NetworkInterface.h>

namespace web { namespace rest { namespace service {

   std::string CSystem::m_restKeyword= std::string("system");

   CSystem::CSystem(boost::shared_ptr<CSystemInformation> systemInformation)
      :m_systemInformation(systemInformation)
   {
      
   }


   CSystem::~CSystem()
   {
   }


   void CSystem::configureDispatcher(CRestDispatcher & dispatcher)
   {
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("binding")("*"), CSystem::getBinding);
      REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("information"), CSystem::getSystemInformation);
   }


   const std::string & CSystem::getRestKeyword()
   {
      return m_restKeyword;
   }

   shared::CDataContainer CSystem::getBinding(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      if (parameters.size() >= 2)
      {
         std::string query = parameters[2];

         if (boost::iequals(query, "SerialPorts"))
            return getSerialPorts();
         else if (boost::iequals(query, "NetworkInterfaces"))
            return getNetworkInterfaces();
         else
            return web::rest::CResult::GenerateError("unsupported binding query : " + query);
      }

      return web::rest::CResult::GenerateError("Cannot retreive url parameters");
   }

   shared::CDataContainer CSystem::getSerialPorts()
   {
      try
      {
         std::vector< shared::CDataContainer > internalList;
         const boost::shared_ptr<const shared::CPeripherals::SerialPortsMap> map = shared::CPeripherals::getSerialPorts();

         shared::CDataContainer result2;
         for(shared::CPeripherals::SerialPortsMap::const_iterator i = map->begin(); i != map->end(); ++i)
         {
            result2.set(i->first, i->second, 0x00); //in case of key contains a dot, just ensure the full key is taken into account
         }

         return web::rest::CResult::GenerateSuccess(result2);
      }
      catch(std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch(...)
      {
         return web::rest::CResult::GenerateError("unknown exception in retreiving all serial ports");
      }
   }

   shared::CDataContainer CSystem::getNetworkInterfaces()
   {
      try
      {
         shared::CDataContainer result;
         Poco::Net::NetworkInterface::NetworkInterfaceList netlist = Poco::Net::NetworkInterface::list();
         for (Poco::Net::NetworkInterface::NetworkInterfaceList::iterator nit = netlist.begin(); nit != netlist.end(); ++nit)
         {
            result.set(nit->address().toString(), (boost::format("%1% (%2%)") % nit->displayName() % nit->address().toString()).str(), 0x00); //in case of key contains a dot, just ensure the full key is taken into account
         }
         return web::rest::CResult::GenerateSuccess(result);
      }
      catch (std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch (...)
      {
         return web::rest::CResult::GenerateError("unknown exception in retreiving all serial ports");
      }
   }
   
   shared::CDataContainer CSystem::getSystemInformation(const std::vector<std::string> & parameters, const shared::CDataContainer & requestContent)
   {
      try
      {
         shared::CDataContainer result;

         result.set("runningPlatform", m_systemInformation->getOperatingSystemName());
         result.set("yadomsVersion", m_systemInformation->getSoftwareVersion().toString());
         result.set("startupTime", m_systemInformation->getStartupDateTime());
         return web::rest::CResult::GenerateSuccess(result);
      }
      catch (std::exception &ex)
      {
         return web::rest::CResult::GenerateError(ex);
      }
      catch (...)
      {
         return web::rest::CResult::GenerateError("unknown exception in retreiving system information");
      }
   }

} //namespace service
} //namespace rest
} //namespace web 
