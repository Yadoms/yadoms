#include "stdafx.h"
#include "System.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/RestDispatcher.h"
#include "web/rest/Result.h"
#include "tools/OperatingSystem.h"
#include <shared/Peripherals.h>
#include <Poco/Net/NetworkInterface.h>
#include <shared/ServiceLocator.h>
#include <startupOptions/IStartupOptions.h>

namespace web
{
   namespace rest
   {
      namespace service
      {
         std::string CSystem::m_restKeyword = std::string("system");

         CSystem::CSystem()
            : m_runningInformation(shared::CServiceLocator::instance().get<IRunningInformation>())
         {
         }


         CSystem::~CSystem()
         {
         }


         void CSystem::configureDispatcher(CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("binding")("*"), CSystem::getBinding);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("information"), CSystem::getSystemInformation);
         }


         const std::string& CSystem::getRestKeyword()
         {
            return m_restKeyword;
         }


         shared::CDataContainer CSystem::getBinding(const std::vector<std::string>& parameters, const std::string& requestContent) const
         {
            if (parameters.size() > 2)
            {
               auto query = parameters[2];

               if (boost::iequals(query, "SerialPorts"))
                  return getSerialPorts();
               if (boost::iequals(query, "NetworkInterfaces"))
                  return getNetworkInterfaces(true);
               if (boost::iequals(query, "NetworkInterfacesWithoutLoopback"))
                  return getNetworkInterfaces(false);
               if (boost::iequals(query, "platformIsWindows"))
                  return platformIs("windows");
               if (boost::iequals(query, "platformIsLinux"))
                  return platformIs("linux");
               if (boost::iequals(query, "platformIsMac"))
                  return platformIs("mac");
               return CResult::GenerateError("unsupported binding query : " + query);
            }

            return CResult::GenerateError("Cannot retreive url parameters");
         }

         shared::CDataContainer CSystem::getSerialPorts() const
         {
            try
            {
               const auto serialPorts = shared::CPeripherals::getSerialPorts();

               shared::CDataContainer result;
               for (const auto& serialPort : *serialPorts)
               {
                  result.set(serialPort.first, serialPort.second, 0x00); //in case of key contains a dot, just ensure the full key is taken into account
               }

               return CResult::GenerateSuccess(result);
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retreiving all serial ports");
            }
         }

         shared::CDataContainer CSystem::getNetworkInterfaces(bool includeLoopback) const
         {
            try
            {
               shared::CDataContainer result;
               auto netlist = Poco::Net::NetworkInterface::list();
               for (const auto& nit : netlist)
               {
                  if (includeLoopback || nit.address().isLoopback())
                     result.set(nit.name(), (boost::format("%1% (%2%)") % nit.displayName() % nit.address().toString()).str(), 0x00); //in case of key contains a dot, just ensure the full key is taken into account
               }
               return CResult::GenerateSuccess(result);
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retreiving all serial ports");
            }
         }

         shared::CDataContainer CSystem::getSystemInformation(const std::vector<std::string>& parameters,
                                                              const std::string& requestContent) const
         {
            try
            {
               shared::CDataContainer result;
               result.set("runningPlatform", m_runningInformation->getOperatingSystemName());
               result.set("yadoms", m_runningInformation->getSoftwareVersion().getContainer());
               result.set("startupTime", m_runningInformation->getStartupDateTime());
               result.set("executablePath", m_runningInformation->getExecutablePath());
               result.set("serverReady", m_runningInformation->isServerFullyLoaded());

               if (shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>()->getDeveloperMode())
                  result.set("developerMode", "true");

               return CResult::GenerateSuccess(result);
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retreiving system information");
            }
         }

         shared::CDataContainer CSystem::platformIs(const std::string& refPlatform) const
         {
            try
            {
               shared::CDataContainer result;
               result.set("result", tools::COperatingSystem::getName() == refPlatform);
               return CResult::GenerateSuccess(result);
            }
            catch (std::exception& ex)
            {
               return CResult::GenerateError(ex);
            }
            catch (...)
            {
               return CResult::GenerateError("unknown exception in retreiving system information");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 


