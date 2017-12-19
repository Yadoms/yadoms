#include "stdafx.h"
#include "System.h"
#include "web/rest/RestDispatcherHelpers.hpp"
#include "web/rest/RestDispatcher.h"
#include "web/rest/Result.h"
#include "tools/OperatingSystem.h"
#include <shared/Peripherals.h>
#include <shared/currentTime/Provider.h>
#include <Poco/Net/NetworkInterface.h>
#include <shared/ServiceLocator.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <startupOptions/IStartupOptions.h>
#include "dateTime/TimeZoneDatabase.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         std::string CSystem::m_restKeyword = std::string("system");
         shared::CDataContainer CSystem::m_virtualDevicesSupportedCapacities;

         CSystem::CSystem(const boost::shared_ptr<dateTime::CTimeZoneDatabase> timezoneDatabase)
            : m_runningInformation(shared::CServiceLocator::instance().get<IRunningInformation>()),
              m_timezoneDatabase(timezoneDatabase)
         {
         }


         CSystem::~CSystem()
         {
         }


         void CSystem::configureDispatcher(CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("binding")("*"), CSystem::getBinding);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("information"), CSystem::getSystemInformation
            );
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("currentTime"), CSystem::getCurrentTime);
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("virtualDevicesSupportedCapacities"), CSystem
               ::getVirtualDevicesSupportedCapacities);
         }


         const std::string& CSystem::getRestKeyword()
         {
            return m_restKeyword;
         }


         shared::CDataContainer CSystem::getBinding(const std::vector<std::string>& parameters,
                                                    const std::string& requestContent) const
         {
            if (parameters.size() > 2)
            {
               const auto query = parameters[2];

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
               if (boost::iequals(query, "supportedTimezones"))
                  return getSupportedTimezones();
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
                  result.set(serialPort.first, serialPort.second, 0x00);
                  //in case of key contains a dot, just ensure the full key is taken into account
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

         shared::CDataContainer CSystem::getNetworkInterfaces(const bool includeLoopback) const
         {
            try
            {
               shared::CDataContainer result;
               auto netlist = Poco::Net::NetworkInterface::list();
               for (const auto& nit : netlist)
               {
                  if (includeLoopback || nit.address().isLoopback())
                     result.set(nit.name(),
                                (boost::format("%1% (%2%)") % nit.displayName() % nit.address().toString()).str(),
                                0x00); //in case of key contains a dot, just ensure the full key is taken into account
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

               if (shared::CServiceLocator::instance().get<const startupOptions::IStartupOptions>()->getDeveloperMode())
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

         shared::CDataContainer CSystem::getCurrentTime(const std::vector<std::string>& parameters,
                                                        const std::string& requestContent) const
         {
            try
            {
               shared::CDataContainer result;
               result.set("now", shared::currentTime::Provider().now());
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

         shared::CDataContainer CSystem::getVirtualDevicesSupportedCapacities(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               shared::CDataContainer result;
               result.set("capacities", getVirtualDevicesSupportedCapacities());
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

         void CSystem::addVirtualDevicesSupportedCapacity(const shared::plugin::yPluginApi::CStandardCapacity& capacity)
         {
            auto capacityContainer = boost::make_shared<shared::CDataContainer>();
            capacityContainer->set("name", capacity.getName());
            capacityContainer->set("unit", capacity.getUnit());
            capacityContainer->set("dataType", capacity.getType());
            m_virtualDevicesSupportedCapacities.set(capacity.getName(), capacityContainer);
         }

         const shared::CDataContainer& CSystem::getVirtualDevicesSupportedCapacities()
         {
            if (m_virtualDevicesSupportedCapacities.empty())
            {
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::ApparentPower());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::ArmingAlarm());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::BatteryLevel());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::CameraMove());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::ColorRGB());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::ColorRGBW());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Counter());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Current());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Curtain());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Dimmable());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Direction());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Distance());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Duration());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Energy());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Event());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Frequency());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Humidity());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Illumination());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Load());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Power());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::PowerFactor());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Pressure());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Rain());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::RainRate());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Rssi());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Speed());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Switch());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Temperature());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Text());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::UpDownStop());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Uv());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Voltage());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Volume());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::WeatherCondition());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Weight());
            }

            return m_virtualDevicesSupportedCapacities;
         }

         shared::CDataContainer CSystem::getSupportedTimezones() const
         {
            try
            {
               shared::CDataContainer result;
               for (const auto& supportedTimezone : m_timezoneDatabase->allIds())
               {
                  result.set(supportedTimezone, supportedTimezone);
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
      } //namespace service
   } //namespace rest
} //namespace web 
