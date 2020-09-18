#pragma once
#include "IRestService.h"
#include "web/rest/RestDispatcher.h"
#include "hardware/usb/IDevicesLister.h"
#include "hardware/serial/ISerialPortsLister.h"
#include "IRunningInformation.h"
#include <shared/plugin/yPluginApi/StandardCapacity.h>
#include "dateTime/TimeZoneDatabase.h"
#include "shared/plugin/yPluginApi/MeasureType.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CSystem : public IRestService
         {
         public:
            explicit CSystem(boost::shared_ptr<dateTime::CTimeZoneDatabase> timezoneDatabase,
                             boost::shared_ptr<hardware::usb::IDevicesLister> usbDevicesLister,
                             boost::shared_ptr<hardware::serial::ISerialPortsLister> serialPortsLister);
            virtual ~CSystem() = default;

            // IRestService implementation
            void configureDispatcher(CRestDispatcher& dispatcher) override;
            // [END] IRestService implementation

            static const std::string& getRestKeyword();


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
            boost::shared_ptr<shared::serialization::IDataSerializable> getSerialPorts() const;
            boost::shared_ptr<shared::serialization::IDataSerializable> getUsbDevices(
               const std::string& listUsbDeviceRequest) const;
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
            static shared::CDataContainer m_virtualDevicesSupportedCapacities;
            boost::shared_ptr<dateTime::CTimeZoneDatabase> m_timezoneDatabase;
            boost::shared_ptr<hardware::usb::IDevicesLister> m_usbDevicesLister;
            boost::shared_ptr<hardware::serial::ISerialPortsLister> m_serialPortsLister;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
