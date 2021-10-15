#pragma once
#include "IRestService.h"
#include "hardware/usb/IDevicesLister.h"
#include "IRunningInformation.h"
#include <shared/plugin/yPluginApi/StandardCapacity.h>

#include "dataAccessLayer/IConfigurationManager.h"
#include "dateTime/TimeZoneDatabase.h"
#include "shared/plugin/yPluginApi/MeasureType.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CSystem final : public IRestService
         {
         public:
            explicit CSystem(boost::shared_ptr<dateTime::CTimeZoneDatabase> timezoneDatabase,
                             boost::shared_ptr<hardware::usb::IDevicesLister> usbDevicesLister,
                             boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager);
            ~CSystem() override = default;

            // IRestService implementation
            void configurePocoDispatcher(poco::CRestDispatcher& dispatcher) override;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> endPoints() override;
            // [END] IRestService implementation


            boost::shared_ptr<shared::serialization::IDataSerializable> getBinding(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getSystemInformation(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getCurrentTime(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getVirtualDevicesSupportedCapacities(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

         private:
            boost::shared_ptr<IAnswer> getSystemInformationV2(boost::shared_ptr<IRequest> request) const;

            boost::shared_ptr<shared::serialization::IDataSerializable> getSerialPorts() const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getUsbDevices(
               const std::string& requestContent) const;
            boost::shared_ptr<shared::serialization::IDataSerializable>
            getNetworkInterfaces(bool includeLoopback) const;
            boost::shared_ptr<shared::serialization::IDataSerializable>
            platformIs(const std::string& refPlatform) const;
            static void addVirtualDevicesSupportedCapacity(
               const shared::plugin::yPluginApi::CStandardCapacity& capacity,
               const std::vector<shared::plugin::yPluginApi::EMeasureType>&
                  acceptedMeasureTypes = std::vector<shared::plugin::yPluginApi::EMeasureType>());
            static const shared::CDataContainer& getVirtualDevicesSupportedCapacities();
            boost::shared_ptr<shared::serialization::IDataSerializable> getSupportedTimezones() const;

            static std::string m_restKeyword;

            boost::shared_ptr<IRunningInformation> m_runningInformation;
            boost::shared_ptr<dataAccessLayer::IConfigurationManager> m_configurationManager;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> m_endPoints;

            static shared::CDataContainer m_virtualDevicesSupportedCapacities;

            boost::shared_ptr<dateTime::CTimeZoneDatabase> m_timezoneDatabase;
            boost::shared_ptr<hardware::usb::IDevicesLister> m_usbDevicesLister;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
