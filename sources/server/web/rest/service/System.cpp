#include "stdafx.h"
#include "System.h"

#include <regex>

#include "tools/OperatingSystem.h"
#include "SerialPortsLister.h"
#include <shared/currentTime/Provider.h>
#include <Poco/Net/NetworkInterface.h>
#include <shared/ServiceLocator.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <startupOptions/IStartupOptions.h>

#include <utility>

#include "RestEndPoint.h"
#include "dateTime/TimeZoneDatabase.h"
#include "web/poco/RestDispatcherHelpers.hpp"
#include "web/poco/RestResult.h"
#include "web/rest/ErrorAnswer.h"
#include "web/rest/NoContentAnswer.h"
#include "web/rest/SuccessAnswer.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         std::string CSystem::m_restKeyword = std::string("system");
         shared::CDataContainer CSystem::m_virtualDevicesSupportedCapacities;

         CSystem::CSystem(const boost::shared_ptr<dateTime::CTimeZoneDatabase> timezoneDatabase,
                          boost::shared_ptr<hardware::usb::IDevicesLister> usbDevicesLister,
                          boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager)
            : m_runningInformation(shared::CServiceLocator::instance().get<IRunningInformation>()),
              m_configurationManager(std::move(configurationManager)),
              m_timezoneDatabase(timezoneDatabase),
              m_usbDevicesLister(std::move(usbDevicesLister))
         {
         }


         void CSystem::configurePocoDispatcher(poco::CRestDispatcher& dispatcher)
         {
            REGISTER_DISPATCHER_HANDLER(dispatcher, "POST", (m_restKeyword)("binding")("*"), CSystem::getBinding)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("information"), CSystem::getSystemInformation)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("currentTime"), CSystem::getCurrentTime)
            REGISTER_DISPATCHER_HANDLER(dispatcher, "GET", (m_restKeyword)("virtualDevicesSupportedCapacities"),
                                        CSystem::getVirtualDevicesSupportedCapacities)
         }


         boost::shared_ptr<shared::serialization::IDataSerializable> CSystem::getBinding(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            if (parameters.size() > 2)
            {
               const auto query = parameters[2];

               if (boost::iequals(query, "serialPorts"))
                  return getSerialPorts();
               if (boost::iequals(query, "usbDevices"))
                  return getUsbDevices(requestContent);
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
               return poco::CRestResult::GenerateError("unsupported binding query : " + query);
            }

            return poco::CRestResult::GenerateError("Cannot retrieve url parameters");
         }

         boost::shared_ptr<shared::CDataContainer> CSystem::getSerialPorts() const
         {
            try
            {
               YADOMS_LOG(debug) << "List serial ports...";
               const auto serialPorts = hardware::serial::CSerialPortsLister::listSerialPorts();

               shared::CDataContainer result;
               for (const auto& serialPort : *serialPorts)
               {
                  result.set(serialPort.first, serialPort.second, 0x00);
                  //in case of key contains a dot, just ensure the full key is taken into account
               }

               return poco::CRestResult::GenerateSuccess(result);
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in retrieving all serial ports");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CSystem::getUsbDevices(const std::string& requestContent) const
         {
            try
            {
               const auto request = shared::CDataContainer::make(requestContent);

               auto existingDevices = m_usbDevicesLister->listUsbDevices();
               YADOMS_LOG(debug) << "USB existing devices :";
               for (const auto& device : existingDevices)
               {
                  YADOMS_LOG(debug) << "  - "
                     << "vid=" << device->vendorId()
                     << ", pid=" << device->productId()
                     << ", name=" << device->yadomsFriendlyName()
                     << ", connectionId=" << device->nativeConnectionString()
                     << ", serial=" << device->serialNumber();
               }

               // If request content is empty, return all existing USB devices
               if (request->empty())
               {
                  shared::CDataContainer result;
                  for (const auto& device : existingDevices)
                     result.set(device->nativeConnectionString(), device->yadomsFriendlyName(), 0x00);
                  //in case of key contains a dot, just ensure the full key is taken into account
                  return poco::CRestResult::GenerateSuccess(result);
               }

               // Filter USB devices by request content

               const auto requestedDevices = request->get<std::vector<boost::shared_ptr<shared::CDataContainer>>>("oneOf");
               shared::CDataContainer result;
               YADOMS_LOG(debug) << "USB requested devices :";
               for (const auto& requestedDevice : requestedDevices)
               {
                  YADOMS_LOG(debug) << "  - "
                     << "vid=" << requestedDevice->get<int>("vendorId")
                     << ", pid=" << requestedDevice->get<int>("productId");

                  for (const auto& existingDevice : existingDevices)
                  {
                     if (existingDevice->vendorId() == requestedDevice->get<int>("vendorId")
                        && existingDevice->productId() == requestedDevice->get<int>("productId"))
                     {
                        //in case of key contains a dot, just ensure the full key is taken into account
                        result.set(existingDevice->nativeConnectionString(), existingDevice->yadomsFriendlyName(),
                                   0x00);
                     }
                  }
               }

               return poco::CRestResult::GenerateSuccess(result);
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in retrieving filtered USB devices");
            }
         }

         boost::shared_ptr<shared::CDataContainer> CSystem::getUsbDevicesV2(const std::vector<std::pair<int, int>>& requestedUsbDevices) const
         {
            const auto existingDevices = m_usbDevicesLister->listUsbDevices();
            YADOMS_LOG(debug) << "USB existing devices :";
            for (const auto& device : existingDevices)
            {
               YADOMS_LOG(debug) << "  - "
                  << "vid=" << device->vendorId()
                  << ", pid=" << device->productId()
                  << ", name=" << device->yadomsFriendlyName()
                  << ", connectionId=" << device->nativeConnectionString()
                  << ", serial=" << device->serialNumber();
            }

            // If request content is empty, return all existing USB devices
            if (requestedUsbDevices.empty())
            {
               auto result = boost::make_shared<shared::CDataContainer>();
               for (const auto& device : existingDevices)
                  //in case of key contains a dot, just ensure the full key is taken into account
                  result->set(device->nativeConnectionString(), device->yadomsFriendlyName(), 0x00);
               return result;
            }

            // Filter USB devices by request content
            auto result = boost::make_shared<shared::CDataContainer>();
            YADOMS_LOG(debug) << "USB requested devices :";
            for (const auto& usbDevice : requestedUsbDevices)
            {
               YADOMS_LOG(debug) << "  - "
                  << "vid=" << usbDevice.first
                  << ", pid=" << usbDevice.second;

               for (const auto& existingDevice : existingDevices)
               {
                  if (existingDevice->vendorId() == usbDevice.first
                     && existingDevice->productId() == usbDevice.second)
                  {
                     //in case of key contains a dot, just ensure the full key is taken into account
                     result->set(existingDevice->nativeConnectionString(), existingDevice->yadomsFriendlyName(),
                                 0x00);
                  }
               }
            }

            return result;
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CSystem::getNetworkInterfaces(
            const bool includeLoopback) const
         {
            try
            {
               shared::CDataContainer result;
               const auto networkInterfaces = Poco::Net::NetworkInterface::list();
               for (const auto& nit : networkInterfaces)
               {
                  if (nit.address().isLoopback() && !includeLoopback)
                     continue;

                  result.set(nit.name(),
                             (boost::format("%1% (%2%)") % nit.displayName() % nit.address().toString()).str(),
                             0x00); //in case of key contains a dot, just ensure the full key is taken into account
               }
               return poco::CRestResult::GenerateSuccess(result);
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in retrieving all serial ports");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CSystem::getSystemInformation(
            const std::vector<std::string>& parameters,
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

               return poco::CRestResult::GenerateSuccess(result);
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in retrieving system information");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CSystem::getCurrentTime(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               shared::CDataContainer result;
               result.set("now", shared::currentTime::Provider().now());
               return poco::CRestResult::GenerateSuccess(result);
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in retrieving system information");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CSystem::getVirtualDevicesSupportedCapacities(
            const std::vector<std::string>& parameters,
            const std::string& requestContent) const
         {
            try
            {
               shared::CDataContainer result;
               result.set("capacities", getVirtualDevicesSupportedCapacities());
               return poco::CRestResult::GenerateSuccess(result);
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in retrieving system information");
            }
         }

         boost::shared_ptr<shared::serialization::IDataSerializable> CSystem::platformIs(
            const std::string& refPlatform) const
         {
            try
            {
               shared::CDataContainer result;
               result.set("result", tools::COperatingSystem::getName() == refPlatform);
               return poco::CRestResult::GenerateSuccess(result);
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in retrieving system information");
            }
         }

         void CSystem::addVirtualDevicesSupportedCapacity(const shared::plugin::yPluginApi::CStandardCapacity& capacity,
                                                          const std::vector<shared::plugin::yPluginApi::EMeasureType>&
                                                          acceptedMeasureTypes)
         {
            const auto capacityContainer = shared::CDataContainer::make();
            capacityContainer->set("name", capacity.getName());
            capacityContainer->set("unit", capacity.getUnit());
            capacityContainer->set("dataType", capacity.getType());

            if (!acceptedMeasureTypes.empty())
            {
               std::vector<std::string> strAcceptedMeasureTypes;
               std::transform(acceptedMeasureTypes.begin(), acceptedMeasureTypes.end(),
                              std::back_inserter(strAcceptedMeasureTypes),
                              [](const auto& acceptedMeasureType) -> std::string
                              {
                                 return acceptedMeasureType.toString();
                              });

               capacityContainer->set("acceptedMeasureTypes", strAcceptedMeasureTypes);
            }

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
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Counter(),
                                                  std::vector<shared::plugin::yPluginApi::EMeasureType>
                                                  {
                                                     shared::plugin::yPluginApi::EMeasureType::kAbsolute,
                                                     shared::plugin::yPluginApi::EMeasureType::kIncrement,
                                                     shared::plugin::yPluginApi::EMeasureType::kCumulative
                                                  });
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Current());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Curtain());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Dimmable());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Direction());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Distance());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Duration());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Energy(),
                                                  std::vector<shared::plugin::yPluginApi::EMeasureType>
                                                  {
                                                     shared::plugin::yPluginApi::EMeasureType::kAbsolute,
                                                     shared::plugin::yPluginApi::EMeasureType::kIncrement,
                                                     shared::plugin::yPluginApi::EMeasureType::kCumulative
                                                  });
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Event());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Frequency());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Humidity());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Illumination());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Load());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Power());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::PowerFactor());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Pressure());
               addVirtualDevicesSupportedCapacity(shared::plugin::yPluginApi::CStandardCapacities::Rain(),
                                                  std::vector<shared::plugin::yPluginApi::EMeasureType>
                                                  {
                                                     shared::plugin::yPluginApi::EMeasureType::kIncrement,
                                                     shared::plugin::yPluginApi::EMeasureType::kCumulative
                                                  });
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

         boost::shared_ptr<shared::serialization::IDataSerializable> CSystem::getSupportedTimezones() const
         {
            try
            {
               shared::CDataContainer result;
               for (const auto& supportedTimezone : m_timezoneDatabase->allIds())
               {
                  result.set(supportedTimezone, supportedTimezone);
               }

               return poco::CRestResult::GenerateSuccess(result);
            }
            catch (std::exception& ex)
            {
               return poco::CRestResult::GenerateError(ex);
            }
            catch (...)
            {
               return poco::CRestResult::GenerateError("unknown exception in retrieving all serial ports");
            }
         }


         boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> CSystem::endPoints()
         {
            if (m_endPoints != nullptr)
               return m_endPoints;

            m_endPoints = boost::make_shared<std::vector<boost::shared_ptr<IRestEndPoint>>>();
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, m_restKeyword + "/information", getSystemInformationV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, m_restKeyword + "/current-time", getCurrentTimeV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, m_restKeyword + "/supported-timezones", getSupportedTimezonesV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, m_restKeyword + "/virtual-devices-supported-capacities",
                                                 getVirtualDevicesSupportedCapacitiesV2));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, m_restKeyword + "/serial-ports", getSerialPorts));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, m_restKeyword + "/usb-devices", getUsbDevices));
            m_endPoints->push_back(MAKE_ENDPOINT(kGet, m_restKeyword + "/network-interfaces", getNetworkInterfacesV2));

            return m_endPoints;
         }

         boost::shared_ptr<IAnswer> CSystem::getSystemInformationV2(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               const auto fields = request->queryParamAsList("fields");

               shared::CDataContainer result;
               if (fields->empty() || fields->find("platform") != fields->end())
                  result.set("platform", m_runningInformation->getOperatingSystemName());
               if (fields->empty() || fields->find("platform-family") != fields->end())
                  result.set("platform-family", tools::COperatingSystem::getName());
               if (fields->empty() || fields->find("yadomsVersion") != fields->end())
                  result.set("yadomsVersion", m_runningInformation->getSoftwareVersion().getVersion().toString());
               if (fields->empty() || fields->find("startupTime") != fields->end())
                  result.set("startupTime", m_runningInformation->getStartupDateTime());
               if (fields->empty() || fields->find("executablePath") != fields->end())
                  result.set("executablePath", m_runningInformation->getExecutablePath());
               if (fields->empty() || fields->find("serverReady") != fields->end())
                  result.set("serverReady", m_runningInformation->isServerFullyLoaded());
               if (fields->empty() || fields->find("databaseVersion") != fields->end())
                  result.set("databaseVersion", m_configurationManager->getDatabaseVersion());

               if (fields->empty() || fields->find("developerMode") != fields->end())
                  result.set("developerMode", shared::CServiceLocator::instance().get<const startupOptions::IStartupOptions>()->getDeveloperMode());

               if (result.empty())
                  return boost::make_shared<CNoContentAnswer>();

               return boost::make_shared<CSuccessAnswer>(result);
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kNotFound,
                                                       "Fail to get server configuration");
            }
         }

         boost::shared_ptr<IAnswer> CSystem::getCurrentTimeV2(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               shared::CDataContainer container;
               container.set("now", shared::currentTime::Provider().now());
               return boost::make_shared<CSuccessAnswer>(container);
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get server current time");
            }
         }

         boost::shared_ptr<IAnswer> CSystem::getSupportedTimezonesV2(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               const auto& supportedTimezones = m_timezoneDatabase->allIds();

               if (supportedTimezones.empty())
                  return boost::make_shared<CNoContentAnswer>();
               
               const auto filters = request->queryParamAsList("filter");

               if (filters->empty())
               {
                  shared::CDataContainer container;
                  container.set("supportedTimezones", supportedTimezones);
                  return boost::make_shared<CSuccessAnswer>(container);
               }

               std::vector<std::string> result;
               for (const auto& timezone : supportedTimezones)
               {
                  for (const auto& filterValue : *filters)
                  {
                     if (timezone.find(filterValue) == std::string::npos)
                        continue;

                     result.push_back(timezone);
                     break; // Don't add twice
                  }
               }

               shared::CDataContainer container;
               container.set("supportedTimezones", result);
               return boost::make_shared<CSuccessAnswer>(container);
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get supported timezones");
            }
         }

         boost::shared_ptr<IAnswer> CSystem::getVirtualDevicesSupportedCapacitiesV2(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               shared::CDataContainer container;
               container.set("capacities", getVirtualDevicesSupportedCapacities());
               return boost::make_shared<CSuccessAnswer>(container);
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get server current time");
            }
         }

         boost::shared_ptr<IAnswer> CSystem::getSerialPorts(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               YADOMS_LOG(debug) << "List serial ports...";
               const auto serialPorts = hardware::serial::CSerialPortsLister::listSerialPorts();

               shared::CDataContainer result;
               for (const auto& serialPort : *serialPorts)
                  //in case of key contains a dot, just ensure the full key is taken into account
                  result.set(serialPort.first, serialPort.second, 0x00);

               if (result.empty())
                  return boost::make_shared<CNoContentAnswer>();

               shared::CDataContainer container;
               container.set("serialPorts", result);
               return boost::make_shared<CSuccessAnswer>(container);
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get serial ports");
            }
         }

         std::vector<std::pair<int, int>> CSystem::toPairsVector(const std::string& param)
         {
            std::vector<std::pair<int, int>> vector;

            const std::regex pattern(R"(\[([0-9]*)\-([0-9]*)\])");
            std::smatch matches;

            for (auto match = std::sregex_iterator(param.begin(), param.end(), pattern); match != std::sregex_iterator(); ++match)
            {
               const auto result = *match;
               vector.emplace_back(std::stoul((*match)[1]),
                                   std::stoul((*match)[2]));
            }

            return vector;
         }

         boost::shared_ptr<IAnswer> CSystem::getUsbDevices(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               const auto filter = request->queryParam("vid-pid", std::string());

               const auto foundDevices = getUsbDevicesV2(filter.empty() ? std::vector<std::pair<int, int>>() : toPairsVector(filter));

               if (foundDevices->empty())
                  return boost::make_shared<CNoContentAnswer>();

               shared::CDataContainer container;
               container.set("usbDevices", foundDevices);
               return boost::make_shared<CSuccessAnswer>(container);
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get USB devices");
            }
         }

         boost::shared_ptr<IAnswer> CSystem::getNetworkInterfacesV2(boost::shared_ptr<IRequest> request) const
         {
            try
            {
               const auto includeLoopback = request->queryParam("loopback", std::string()) == "include";

               shared::CDataContainer result;
               const auto networkInterfaces = Poco::Net::NetworkInterface::list();
               for (const auto& nit : networkInterfaces)
               {
                  if (nit.address().isLoopback() && !includeLoopback)
                     continue;

                  //in case of key contains a dot, just ensure the full key is taken into account
                  result.set(nit.name(),
                             (boost::format("%1% (%2%)") % nit.displayName() % nit.address().toString()).str(),
                             0x00);
               }

               if (result.empty())
                  return boost::make_shared<CNoContentAnswer>();

               shared::CDataContainer container;
               container.set("networkInterfaces", result);
               return boost::make_shared<CSuccessAnswer>(container);
            }
            catch (const std::exception&)
            {
               return boost::make_shared<CErrorAnswer>(shared::http::ECodes::kInternalServerError,
                                                       "Fail to get network interfaces");
            }
         }
      } //namespace service
   } //namespace rest
} //namespace web 
