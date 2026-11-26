#pragma once
#include "IRestService.h"
#include "hardware/usb/IDevicesLister.h"
#include "IRunningInformation.h"
#include <shared/plugin/yPluginApi/StandardCapacity.h>

#include "dataAccessLayer/IConfigurationManager.h"
#include "database/IDatabaseRequester.h"
#include "dateTime/TimeZoneDatabase.h"
#include "shared/plugin/yPluginApi/MeasureType.h"

namespace web::rest::service
{
   class CSystem final : public IRestService
   {
   public:
      explicit CSystem(boost::shared_ptr<dateTime::CTimeZoneDatabase> timezoneDatabase,
                       boost::shared_ptr<hardware::usb::IDevicesLister> usbDevicesLister,
                       boost::shared_ptr<database::IDatabaseRequester> databaseRequester,
                       boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager);

      CSystem(const CSystem&) = delete;
      CSystem(CSystem&&) = delete;
      CSystem& operator=(const CSystem&) = delete;
      CSystem& operator=(CSystem&&) = delete;

      ~CSystem() override = default;

      // IRestService implementation
      void configurePocoDispatcher(poco::CRestDispatcher& dispatcher) override;
      boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> endPoints() override;
      // [END] IRestService implementation


      boost::shared_ptr<shared::serialization::IDataSerializable> getBindingV1(
         const std::vector<std::string>& parameters,
         const std::string& requestContent) const;
      boost::shared_ptr<shared::serialization::IDataSerializable> getSystemInformationV1(
         const std::vector<std::string>& parameters,
         const std::string& requestContent) const;
      boost::shared_ptr<shared::serialization::IDataSerializable> getCurrentTimeV1(
         const std::vector<std::string>& parameters,
         const std::string& requestContent);
      boost::shared_ptr<shared::serialization::IDataSerializable> getVirtualDevicesSupportedCapacitiesV1(
         const std::vector<std::string>& parameters,
         const std::string& requestContent);

   private:
      boost::shared_ptr<IAnswer> getSystemInformationV2(const boost::shared_ptr<IRequest>& request) const;
      static boost::shared_ptr<IAnswer> getCurrentTimeV2(const boost::shared_ptr<IRequest>& request);
      boost::shared_ptr<IAnswer> getSupportedTimezonesV2(const boost::shared_ptr<IRequest>& request) const;
      boost::shared_ptr<IAnswer> getVirtualDevicesSupportedCapacitiesV2(const boost::shared_ptr<IRequest>& request);
      static boost::shared_ptr<shared::CDataContainer> getSerialPorts();
      static boost::shared_ptr<IAnswer> getSerialPortsV2(const boost::shared_ptr<IRequest>& request);
      boost::shared_ptr<IAnswer> getUsbDevicesV2(const boost::shared_ptr<IRequest>& request) const;
      static boost::shared_ptr<IAnswer> getNetworkInterfacesV2(const boost::shared_ptr<IRequest>& request);
      boost::shared_ptr<IAnswer> getBindingV2(const boost::shared_ptr<IRequest>& request) const;

      static std::vector<std::pair<int, int>> toPairsVector(const std::unique_ptr<std::set<std::string>>& vidPidList);
      boost::shared_ptr<shared::serialization::IDataSerializable> getUsbDevices(const std::string& requestContent) const;
      static boost::shared_ptr<shared::serialization::IDataSerializable> getNetworkInterfaces(bool includeLoopback);
      static boost::shared_ptr<shared::serialization::IDataSerializable> platformIs(const std::string& refPlatform);
      static void addVirtualDevicesSupportedCapacity(const shared::plugin::yPluginApi::CStandardCapacity& capacity,
                                                     const std::vector<shared::plugin::yPluginApi::EMeasureType>&
                                                        acceptedMeasureTypes = std::vector<shared::plugin::yPluginApi::EMeasureType>());
      static const shared::CDataContainer& getVirtualDevicesSupportedCapacities();
      std::shared_ptr<std::vector<shared::plugin::yPluginApi::CStandardCapacity>> getVirtualDevicesSupportedCapacitiesV2();
      boost::shared_ptr<shared::serialization::IDataSerializable> getSupportedTimezones() const;

      inline static const std::string RestKeyword = "system";  // NOLINT(bugprone-throwing-static-initialization)

      boost::shared_ptr<database::IDatabaseRequester> _databaseRequester;
      boost::shared_ptr<IRunningInformation> _runningInformation;
      boost::shared_ptr<dataAccessLayer::IConfigurationManager> _configurationManager;
      boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> _endPoints;

      static shared::CDataContainer _virtualDevicesSupportedCapacities;
      std::shared_ptr<std::vector<shared::plugin::yPluginApi::CStandardCapacity>> _virtualDevicesSupportedCapacitiesV2;

      boost::shared_ptr<dateTime::CTimeZoneDatabase> _timezoneDatabase;
      boost::shared_ptr<hardware::usb::IDevicesLister> _usbDevicesLister;
   };
}
